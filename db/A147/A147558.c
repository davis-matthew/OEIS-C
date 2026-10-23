/*
 * A147558  Result of using the Fibonacci numbers as coefficients in an
 *          infinite polynomial series in x and then expressing this series as
 *          (1+a(1)x)(1+a(2)x^2)(1+a(3)x^3)...
 * https://oeis.org/A147558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is obtained by dividing the Fibonacci‐coefficient series
 *            S(x) = 1 + x + 1·x² + 2·x³ + 3·x⁴ + 5·x⁵ + … (coefficients:
 *            1,1,1,2,3,5,8,13,…) by the infinite product (1 + a(1)x)(1 +
 *            a(2)x²)(1 + a(3)x³).... The coefficient a(n) is the n‑th factor
 *            needed to factor S(x)
 * Range:     u8 n <= 17, u16 n <= 30, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation performs the division sequentially:
   * Begin with the coefficients of S(x) up to degree n.
   * For each i = 1 … n:
       - a(i) = current coefficient at xᶦ.
       - Update coefficients for degrees ≥ i by
         C[k] ← C[k] – a(i) * C[k‑i]   (iterating k ascending so that C[k‑i] is already updated).
   * After processing all i, a(n) is the desired value.
   This algorithm takes O(n²) arithmetic operations and works for any
   build‑time definition of seq_t. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t N = n;
    /* Allocate arrays for coefficients and factor values. */
    seq_t *C = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *A = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (C == NULL || A == NULL) {
        free(C);
        free(A);
        return 0;
    }

    /* Initialize the initial series coefficients:
         C[0] = 1, C[1] = 1, C[k] = Fibonacci(k) for k ≥ 2,
         where Fibonacci(0)=0, Fibonacci(1)=1. */
    C[0] = 1;
    if (N >= 1)
        C[1] = 1;

    seq_t f_prev = 0; /* Fibonacci(k-2) for k=2 */
    seq_t f_curr = 1; /* Fibonacci(k-1) for k=2 */
    for (idx_t k = 2; k <= N; ++k) {
        seq_t f_next = f_prev + f_curr;   /* Fibonacci(k) */
        C[k] = f_next;
        f_prev = f_curr;
        f_curr = f_next;
    }

    /* Sequentially determine a(i) and update the remaining series. */
    for (idx_t i = 1; i <= N; ++i) {
        A[i] = C[i];                     /* a(i) is the current coefficient */
        seq_t ai = A[i];
        for (idx_t k = i; k <= N; ++k) { /* Update coefficients for degrees ≥ i */
            seq_t old = C[k];
            C[k] = old - ai * C[k - i];
        }
    }

    seq_t result = A[N];
    free(C);
    free(A);
    return result;
}
