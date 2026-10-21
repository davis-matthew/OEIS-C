/*
 * A203558  Number of nX2 0..2 arrays with row sums equal and column sums
 *          equal.
 * https://oeis.org/A203558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A203558 – Number of n×2 arrays with entries 0..2, all row sums equal 
 * and column sums equal.
 *
 * Formula: a(n) = 2 + Σ_{k=0}^{⌊n/2⌋} binomial(n,k)*binomial(n-k,n-2k)
 *           + (n even ? 2*binomial(n,n/2) : 0)
 *
 * The term 2 counts the all‑0 and all‑2 arrays.  The summation counts
 * arrays whose row sum is 0 or 2 (k = 0 gives the all‑0 array).  The
 * additional term for even n counts arrays whose row sum is 1 or 3.
 */

#include <stdbool.h> /* for bool, if needed */

static seq_t binomial_calc(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;   /* Use symmetry to reduce loop length */

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply by (n - k + i) and divide by i, both exact */
        seq_t mult_factor = (seq_t)(n - k + i);
        seq_t div_factor  = (seq_t)i;
        result = result * mult_factor;
        result = result / div_factor;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Base count: all‑0 and all‑2 arrays */
    seq_t total = 2;

    /* Summation over k = 0 .. floor(n/2) */
    idx_t kmax = n >> 1;   /* floor(n/2) */
    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t term = binomial_calc(n, k);
        term = term * binomial_calc(n - k, n - 2*k);
        total = total + term;
    }

    /* Additional term for even n: row sums 1 and 3 */
    if ((n & 1) == 0) {
        seq_t mid = binomial_calc(n, n >> 1);
        total = total + (mid << 1);   /* 2 * mid */
    }

    return total;
}
