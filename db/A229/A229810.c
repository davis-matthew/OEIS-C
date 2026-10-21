/*
 * A229810  G.f. C(x) satisfies: C(x) = x + 5*A(x)*B(x), where A(x) = x +
 *          2*B(x)*C(x) and B(x) = x + 3*A(x)*C(x).
 * https://oeis.org/A229810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A229810
 * G.f. C(x) satisfies: C = x + 5*A*B,
 * A = x + 2*B*C, B = x + 3*A*C.
 * The coefficients a(n) of C(x) satisfy
 *   a(1) = 1,
 *   a(n) = 5 * Σ_{i=1}^{n-1} A(i)*B(n-i)    (n ≥ 2)
 * where A,B are defined by the same system with a(1)=b(1)=c(1)=1.
 * The implementation keeps three memoised arrays for A,B,C and fills
 * them on demand.  The arrays are enlarged only as far as required.
 * The algorithm runs in O(n²) time for the n‑th term, easily below
 * the time limit for n ≤ 10 000. */

#include <stdlib.h>

/* seq_t and idx_t are supplied by the build.  They are unsigned integer
 * types of any width; the code uses only integer arithmetic. */
static seq_t *A = NULL;   /* A(n) */
static seq_t *B = NULL;   /* B(n) */
static seq_t *C = NULL;   /* C(n) */
static idx_t  cache_size = 0;   /* number of allocated elements */

/* Ensure that cached arrays have room for indices 0 … n */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    idx_t old_size = cache_size;
    idx_t new_size = old_size ? old_size * 2 : 16;
    while (new_size <= n)
        new_size *= 2;

    seq_t *newA = (seq_t *)realloc(A, new_size * sizeof(seq_t));
    seq_t *newB = (seq_t *)realloc(B, new_size * sizeof(seq_t));
    seq_t *newC = (seq_t *)realloc(C, new_size * sizeof(seq_t));
    if (!newA || !newB || !newC) {
        free(A); free(B); free(C);
        abort();
    }
    A = newA;
    B = newB;
    C = newC;

    /* Initialise newly allocated entries to zero */
    for (idx_t i = old_size; i < new_size; ++i) {
        A[i] = (seq_t)0;
        B[i] = (seq_t)0;
        C[i] = (seq_t)0;
    }
    cache_size = new_size;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0)  /* sequence is defined for n ≥ 1 */
        return (seq_t)0;

    ensure_cache(n);

    /* Base case: A[1] = B[1] = C[1] = 1 */
    if (n >= 1 && C[1] == (seq_t)0) {   /* initialise only once */
        A[1] = (seq_t)1;
        B[1] = (seq_t)1;
        C[1] = (seq_t)1;
    }

    /* If the requested term is already known, return it. */
    if (C[n] != (seq_t)0)
        return C[n];

    /* Compute all terms up to n.  Only terms that are still zero are
     * recomputed.  This guard allows repeated calls with arbitrary
     * ordering of n. */
    for (idx_t k = 2; k <= n; ++k) {
        if (C[k] != (seq_t)0)   /* already computed */
            continue;

        seq_t sum_ab = (seq_t)0;  /* Σ A(i)*B(k-i) */
        seq_t sum_bc = (seq_t)0;  /* Σ B(i)*C(k-i) */
        seq_t sum_ac = (seq_t)0;  /* Σ A(i)*C(k-i) */

        for (idx_t i = 1; i < k; ++i) {
            sum_ab += A[i] * B[k - i];
            sum_bc += B[i] * C[k - i];
            sum_ac += A[i] * C[k - i];
        }

        C[k] = sum_ab * (seq_t)5;
        A[k] = sum_bc * (seq_t)2;
        B[k] = sum_ac * (seq_t)3;
    }

    return C[n];
}
