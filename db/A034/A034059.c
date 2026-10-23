/*
 * A034059  Decimal part of a(n)^(1/4) starts with n (4th powers excluded).
 * https://oeis.org/A034059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest integer that is NOT a perfect fourth power
 * whose fractional part of the fourth root starts with the decimal
 * representation of n (with no fourth‑power excluded).  The interval
 * of possible fractional parts for a given integer t is
 * [t + n/10^k, t + (n+1)/10^k), where k = number of decimal digits of n. */

#include <stdint.h>
#include <stdbool.h>

/* auxiliary: exact fourth power of a seq_t value */
static inline seq_t pow4_seq(seq_t x)
{
    seq_t y = x * x;
    return y * y;
}

/* integer fourth root:  floor(x^(1/4))   (x fits in seq_t) */
static inline idx_t int_fourth_root(seq_t x)
{
    idx_t lo = 0, hi = 1;
    /* raise hi until hi^4 > x */
    while (pow4_seq((seq_t)hi) <= x) {
        hi <<= 1;
    }
    /* binary search */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (pow4_seq((seq_t)mid) <= x) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

/* number of decimal digits of n (>=0) */
static inline int num_digits(idx_t n)
{
    if (n == 0) return 1;
    int d = 0;
    while (n) {
        d++;
        n /= 10;
    }
    return d;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* Determine power of 10 corresponding to number of digits of n */
    int k = num_digits(n);
    unsigned __int128 p = 1;
    for (int i = 0; i < k; ++i) p *= 10;
    unsigned __int128 p4 = p * p * p * p;  /* 10^(4k) */

    /* scan t from 0 upward */
    for (idx_t t = 0;; ++t) {
        /* A = t*p + n   (precise integer) */
        unsigned __int128 A = (unsigned __int128)t * p + n;
        unsigned __int128 B = A + 1;

        /* compute A^4 and B^4 with 128‑bit arithmetic */
        unsigned __int128 A2 = A * A;
        unsigned __int128 A4 = A2 * A2;
        unsigned __int128 B2 = B * B;
        unsigned __int128 B4 = B2 * B2;

        /* lower and upper bounds for m */
        unsigned __int128 LB = (A4 + p4 - 1) / p4;          /* ceil(A4 / p4) */
        unsigned __int128 UB = (B4 - 1) / p4;              /* floor((B4-1)/p4) */

        if (LB > UB) continue; /* no integer in this interval */

        unsigned __int128 candidate = LB;

        /* skip perfect fourth powers (including 0) */
        while (candidate <= UB) {
            if (candidate != 0) {
                seq_t cand_seq = (seq_t)candidate;
                idx_t rt = int_fourth_root(cand_seq);
                if (pow4_seq((seq_t)rt) != cand_seq) {
                    break;  /* found valid candidate */
                }
            }
            ++candidate;
        }

        if (candidate <= UB) {
            return (seq_t)candidate;
        }
        /* otherwise continue with next t */
    }
}
