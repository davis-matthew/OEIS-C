/*
 * A068039  Number of subsets of {1,2,3,...,n} that sum to 0 mod 18.
 * https://oeis.org/A068039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {1,…,n} whose sum ≡ 0 (mod 18)
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 18‑state DP: dp[r] = number of subsets of {1,…,k} whose sum ≡ r (mod 18). */
    static seq_t *seq      = NULL;
    static size_t seq_len  = 0, seq_cap = 0;
    static seq_t dp[18];
    static bool initialized = false;

    /* First call – initialise */
    if (!initialized) {
        seq_cap = 1;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        dp[0] = (seq_t)1;                /* empty set */
        for (int i = 1; i < 18; ++i) dp[i] = (seq_t)0;
        seq[0] = dp[0];                  /* a(0) */
        seq_len = 1;
        initialized = true;
    }

    /* Extend precomputation up to n if necessary */
    if ((idx_t)seq_len <= n) {
        size_t need = (size_t)(n + 1);
        if (need > seq_cap) {
            size_t new_cap = seq_cap ? seq_cap : 1;
            while (new_cap < need) new_cap <<= 1;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            seq_cap = new_cap;
        }

        for (idx_t cur = seq_len; cur <= n; ++cur) {
            seq_t newdp[18];

            /* copy current dp into newdp */
            for (int j = 0; j < 18; ++j) newdp[j] = dp[j];

            /* add subsets that include element cur */
            int shift = (int)(cur % 18);   /* cur mod 18 fits in int */
            for (int j = 0; j < 18; ++j) {
                int dest = (j + shift) % 18;
                newdp[dest] = newdp[dest] + dp[j];
            }

            /* store new dp */
            for (int j = 0; j < 18; ++j) dp[j] = newdp[j];

            /* record a(cur) */
            seq[seq_len] = dp[0];
            ++seq_len;
        }
    }
    return seq[n];
}
