/*
 * A137464  Concatenation of n and a list of the divisors of n.
 * https://oeis.org/A137464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, less
 * Formula:   a(n) = concatenation of n followed by all positive divisors of n
 *            in ascending order
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A137464 */

#include <stddef.h>

/* integer square root of an unsigned 64‑bit integer */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > n / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* number of decimal digits of a non‑zero value */
static inline idx_t digit_len(idx_t x)
{
    idx_t len = 0;
    do {
        ++len;
        x /= 10;
    } while (x);
    return len;
}

/* 10^e as a seq_t (e is small, at most a few decimal digits) */
static inline seq_t pow10_seq(idx_t e)
{
    seq_t p = 1;
    for (idx_t i = 0; i < e; ++i)
        p *= 10;
    return p;
}

/* main sequence routine */
seq_t a(idx_t n)
{
    seq_t result = (seq_t)n;          /* start with n itself          */

    /* maxima for divisor enumeration */
    idx_t limit = isqrt_u64(n);

    /* we need somewhere to hold the larger divisors found in the pairs */
    /* n <= 10000 ⇒ divisor count < 128; allocate a small buffer      */
    idx_t big[256];
    size_t bigcnt = 0;

    /* collect small divisors (i) and corresponding large ones (n/i) */
    for (idx_t i = 1; i <= limit; ++i) {
        if (n % i == 0) {
            /* append small divisor i */
            idx_t d = i;
            seq_t mul = pow10_seq(digit_len(d));
            result = result * mul + (seq_t)d;

            /* store large divisor if distinct */
            idx_t dv = n / i;
            if (dv != i) {
                if (bigcnt < sizeof(big)/sizeof(big[0]))
                    big[bigcnt++] = dv;
            }
        }
    }

    /* append the large divisors in ascending order:
       they were collected in reverse order by the pair loop.       */
    for (size_t j = 0; j < bigcnt; ++j) {
        /* process in reverse order to maintain ascending sequence   */
        idx_t d = big[bigcnt - 1 - j];
        seq_t mul = pow10_seq(digit_len(d));
        result = result * mul + (seq_t)d;
    }

    return result;
}
