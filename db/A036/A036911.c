/*
 * A036911  a(n) = (binomial(4*n, 2*n) + (-1)^n*binomial(2*n, n)^2)/2.
 * https://oeis.org/A036911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (binomial(4*n, 2*n) + (-1)^n * binomial(2*n, n)^2)/2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses two different strategies depending on the
   width of seq_t: for 64‑bit seq_t it uses 128‑bit temporaries to
   avoid overflow, for arbitrary‑precision seq_t it uses a generic
   multiplicative binomial routine. */

#include <stdint.h>

/* Generic multiplicative binomial (exact, works for arbitrary
   seq_t).  Assumes that seq_t supports the usual arithmetic
   operators and that k <= n. */
static seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    idx_t i;
    for (i = 1; i <= k; ++i) {
        res = (res * (seq_t)(n - i + 1)) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* 64‑bit seq_t: use 128‑bit temporaries for safe arithmetic. */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        __int128 two_n  = ( __int128 )n * 2;
        __int128 four_n = two_n * 2;

        /* binom(2n, n) */
        __int128 b = 1;
        idx_t i;
        for (i = 1; i <= n; ++i) {
            b = ( b * ( two_n - i + 1 ) ) / i;
        }
        __int128 b_sq = b * b;

        /* binom(4n, 2n) */
        __int128 c = 1;
        for (i = 1; i <= (idx_t)two_n; ++i) {
            c = ( c * ( four_n - i + 1 ) ) / i;
        }

        /* numerator = c + (-1)^n * b_sq */
        __int128 num;
        if ((n & 1ULL) == 0)
            num = c + b_sq;
        else
            num = c - b_sq;

        return (seq_t)(num >> 1);
    }

    /* arbitrary‑precision seq_t: compute directly with binom_calc. */
    idx_t two_n  = n + n;
    idx_t four_n = two_n + two_n;

    seq_t b  = binom_calc(two_n, n);       /* binomial(2n, n)   */
    seq_t b_sq = b * b;                    /* square of the binomial */
    seq_t c   = binom_calc(four_n, two_n); /* binomial(4n, 2n)  */

    seq_t numerator;
    if ((n & 1ULL) == 0)
        numerator = c + b_sq;
    else
        numerator = c - b_sq;

    return numerator >> 1;
}
