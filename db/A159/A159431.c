/*
 * A159431  Number of n X n arrays of squares of integers summing to 33.
 * https://oeis.org/A159431
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×n arrays of squares of integers summing to 33
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the enumeration of all solutions to 33 = Σ s_i·c_i where
   s_i ∈ {1,4,9,16,25} and counts c_i are non‑negative integers.  For each
   solution the number of arrays equals C(n², t) × (t! / ∏ c_i!) where
   t = Σ c_i.  The function evaluates this sum for the given n. */
#include <stdbool.h>

static seq_t binom_small(seq_t N, idx_t k)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiplier: N - (k-i) = N - k + i */
        seq_t num = N - (seq_t)(k - i);
        res = res * num;
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* values below offset are irrelevant */

    idx_t N_int = n * n;                /* n² as idx_t (small, fits 64‑bit) */
    seq_t N_seq = (seq_t)n * (seq_t)n;

    seq_t total = 0;

    /* Enumerate all solutions of 33 = 1·a1 + 4·a4 + 9·a9 + 16·a16 + 25·a25 */
    for (idx_t a25 = 0; a25 <= 1; ++a25) {
        idx_t rem1 = 33 - 25 * a25;
        if (rem1 > 33) continue;            /* impossible when a25=1: rem1=8 */

        for (idx_t a16 = 0; a16 <= 2; ++a16) {
            idx_t rem2 = rem1 - 16 * a16;
            if (rem2 > rem1) continue;

            for (idx_t a9 = 0; a9 <= 3; ++a9) {
                idx_t rem3 = rem2 - 9 * a9;
                if (rem3 > rem2) continue;

                for (idx_t a4 = 0; a4 <= 8; ++a4) {
                    idx_t rem4 = rem3 - 4 * a4;
                    if (rem4 > rem3) continue;
                    if (rem4 > 33) continue;   /* impossible */

                    idx_t a1 = rem4;           /* remaining sum, automatically >=0 */

                    idx_t t = a1 + a4 + a9 + a16 + a25;
                    if (t > N_int) continue;   /* not enough positions to place non‑zeros */

                    /* multinomial coefficient: t! / (a1!·a4!·a9!·a16!·a25!) */
                    seq_t multinom = 1;
                    idx_t remaining = t;
                    if (a1) {
                        seq_t b = binom_small((seq_t)remaining, a1);
                        multinom *= b;
                        remaining -= a1;
                    }
                    if (a4) {
                        seq_t b = binom_small((seq_t)remaining, a4);
                        multinom *= b;
                        remaining -= a4;
                    }
                    if (a9) {
                        seq_t b = binom_small((seq_t)remaining, a9);
                        multinom *= b;
                        remaining -= a9;
                    }
                    if (a16) {
                        seq_t b = binom_small((seq_t)remaining, a16);
                        multinom *= b;
                        remaining -= a16;
                    }
                    if (a25) {
                        seq_t b = binom_small((seq_t)remaining, a25);
                        multinom *= b;
                        remaining -= a25;
                    }
                    /* remaining should now be 0 */

                    /* binomial coefficient C(N, t) */
                    seq_t binom = binom_small(N_seq, t);

                    total += binom * multinom;
                }
            }
        }
    }

    return total;
}
