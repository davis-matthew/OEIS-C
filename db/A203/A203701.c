/*
 * A203701  Vandermonde sequence using x^2 + y^2 applied to the Fibonacci
 *          numbers (1,2,3,5,...F(n+1)).
 * https://oeis.org/A203701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{1 ≤ i < j ≤ n} (F_i^2 + F_j^2), where F_1 = 1, F_2 = 2
 *            and F_k = F_{k-1} + F_{k-2} for k ≥ 3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are defined by the build environment. */
#include <stdlib.h>

static seq_t *fib = NULL;   /* Fibonacci numbers F_1 … F_n */
static seq_t *sq  = NULL;   /* Squares of Fibonacci numbers */
static seq_t *val = NULL;   /* Sequence values a(1) … a(n) */
static size_t  capacity = 0; /* Allocated size of the above arrays */
static idx_t   computed = 0;  /* Largest index for which val[i] has been computed */

/* Ensure that the static arrays hold at least (n+1) elements. */
static void
ensure_capacity(idx_t n)
{
    if (capacity < (size_t)(n + 1)) {
        capacity = n + 1;
        fib = (seq_t *)realloc(fib, capacity * sizeof(seq_t));
        sq  = (seq_t *)realloc(sq,  capacity * sizeof(seq_t));
        val = (seq_t *)realloc(val, capacity * sizeof(seq_t));
        if (!fib || !sq || !val) exit(1);  /* allocation failed */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out of range, but not used */

    ensure_capacity(n);

    /* Initialize base case if needed. */
    if (computed < 1) {
        fib[1] = (seq_t)1;
        sq[1]  = fib[1] * fib[1];
        val[1] = (seq_t)1;
        computed = 1;
    }

    /* Compute Fibonacci numbers and the sequence up to n. */
    for (idx_t k = computed + 1; k <= n; ++k) {
        if (k == 2) {
            fib[2] = (seq_t)2;
        } else {
            fib[k] = fib[k-1] + fib[k-2];
        }
        sq[k] = fib[k] * fib[k];

        /* Product of (F_i^2 + F_k^2) for i = 1 … k-1. */
        seq_t prod = (seq_t)1;
        for (idx_t i = 1; i < k; ++i) {
            seq_t term = sq[i] + sq[k];
            prod = prod * term;
        }

        /* a(k) = a(k-1) * prod. */
        val[k] = val[k-1] * prod;
    }

    computed = n;
    return val[n];
}
