/*
 * A115424  Integers n > 0 such that n XOR 62*n = 63*n.
 * https://oeis.org/A115424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 91, u32 n <= 5102, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A115424
    An integer n satisfies  n XOR 62*n  =  63*n   iff
    no two 1‑bits of n are closer than 6 positions.
    This function returns the n‑th smallest integer having that property
    (indices start at 1). */
#include <stdint.h>
#include <stddef.h>

static idx_t dp[4096];      /* dp[i] = number of subsets of {0,…,i-1}
                               with distance >=6 between selected indices */
static idx_t dp_size = 0;   /* number of entries currently filled */

/* Ensure dp[0 .. up_to] are computed. */
static void ensure_dp(idx_t up_to)
{
    if (dp_size == 0) {
        dp[0] = 1;                /* empty set only */
        dp_size = 1;
    }
    while (dp_size <= up_to) {
        idx_t i = dp_size;         /* compute dp[i] */
        if (i < 6) {
            dp[i] = dp[i-1] + 1;   /* can choose i-1 alone or not */
        } else {
            dp[i] = dp[i-1] + dp[i-6];
        }
        dp_size++;
    }
}

seq_t a(idx_t n)
{
    idx_t h = 0;
    idx_t cumulative = 0;
    idx_t c = 0;                     /* count for current h */

    /* Find highest bit position h whose block contains the nth term */
    for (;; ++h) {
        if (h <= 5) {
            c = 1;                   /* only 2^h itself */
        } else {
            ensure_dp(h - 5);
            c = dp[h - 5];           /* subsets of allowed lower bits */
        }
        cumulative += c;
        if (cumulative >= n) break; /* group of index h found */
    }

    idx_t r = n - (cumulative - c); /* 1‑based rank within the group */

    if (h <= 5) {
        return ((seq_t)1 << h);      /* only 2^h */
    }

    idx_t N = h - 5;                 /* number of allowed lower positions
                                       0 … N-1 */
    ensure_dp(N);                    /* make sure dp[0 … N-1] exist */

    idx_t rr = r;                    /* remaining rank */
    seq_t lower = 0;
    int pos = (int)N - 1;            /* start with most significant allowed
                                       position */

    while (pos >= 0) {
        idx_t count0 = dp[pos];      /* subsets with this bit set to 0 */

        if (rr <= count0) {
            /* bit stays 0 */
            pos--;
        } else {
            /* set bit to 1, skip forbidden lower positions */
            rr -= count0;
            lower |= ((seq_t)1 << pos);
            pos -= 6;                /* positions pos-1 … pos-5 cannot be set */
        }
    }

    return ((seq_t)1 << h) | lower;
}
