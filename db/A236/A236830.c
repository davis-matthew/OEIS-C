/*
 * A236830  Riordan array (1/(1-x*C(x)^3), x*C(x)), C(x) the g.f. of A000108.
 * https://oeis.org/A236830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = row‑wise flattening of the Riordan array T(n,k) * where
 *            T(n,k) = Σ_{i=0..n-k} Fibonacci(2*i-1) * binomial(2*n-2-k-i,
 *            n-k-i)
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 152, u64 n <= 527,
 *            u128 n <= 2015, big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper functions */
#include "../utils/fib.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t start = r * (r + 1) / 2; /* position of T(r,0) in the flattened array */
    idx_t k = n - start;           /* column index within the row */

    seq_t sum = 0;
    idx_t max_i = r - k;           /* number of terms in the summation */

    for (idx_t i = 0; i <= max_i; ++i) {
        seq_t fib_term = (i == 0) ? (seq_t)1 : fib(2 * i - 1);

        idx_t n_bin = 2 * r - 2 - k - i;
        idx_t k_bin = r - k - i;

        seq_t bin;
        if (k_bin == 0) {
            bin = (seq_t)1;                /* binomial(n,- ) for k=0 is 1, even if n<0 */
        } else if (n_bin < k_bin || n_bin < 0) {
            bin = (seq_t)0;                /* binomial is zero for n < k or negative n */
        } else {
            bin = binomial(n_bin, k_bin);
        }

        sum += fib_term * bin;
    }

    return sum;
}
