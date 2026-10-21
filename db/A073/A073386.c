/*
 * A073386  Ninth convolution of A000129(n+1) (generalized (2,1)-Fibonacci,
 *          called Pell numbers), n>=0, with itself.
 * https://oeis.org/A073386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 29, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=0..floor(n/2)} 2^(n-2k) * binomial(n-k+9, 9) * binomial(n-k, k)
 * (OEIS A073386 – ninth convolution of Pell numbers)
 */

#include <stddef.h>

/* Helper: compute binomial coefficient C(n,k) using a multiplicative loop.
   Works for any seq_t that supports +, -, *, / and cast from idx_t. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;          /* use smaller k */

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t k = 0; k <= n / 2; ++k) {
        /* 2^(n-2k) */
        seq_t pow2 = 1;
        idx_t exp = n - 2 * k;
        for (idx_t i = 0; i < exp; ++i) pow2 = pow2 * 2;

        /* C(n-k+9, 9) and C(n-k, k) */
        seq_t c1 = binom(n - k + 9, 9);
        seq_t c2 = binom(n - k, k);

        seq_t term = pow2 * c1 * c2;
        sum += term;
    }

    return sum;
}
