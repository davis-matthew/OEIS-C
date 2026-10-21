/*
 * A230127  Number of binary strings of length n avoiding "squares" (that is,
 *          repeated blocks of the form xx) with |x| > 1.
 * https://oeis.org/A230127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursively counts binary strings avoiding squares
   (repeated blocks xx with |x| > 1) up to length 18.
   For n ≥ 19 the value is 0.  The counts are computed once
   by a brute‑force backtracking because the sequence stops at
   n = 18.  The algorithm checks every new bit against all
   possible square patterns ending at that position. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Forward declaration of helper routine */
static void dfs(uint32_t bits, unsigned len, seq_t *cnt);

/* static array holding the precomputed counts for n = 0 .. 18 */
static seq_t pre_cnt[19] = {0};

/* flag that indicates whether the table has been filled */
static bool precomputed = false;

/* helper routine that performs a depth‑first search over all
   binary strings with length ≤ 18, counting those that avoid
   squares. */
static void dfs(uint32_t bits, unsigned len, seq_t *cnt)
{
    if (len <= 18) {
        /* count the current string (including the empty string) */
        cnt[len] += 1;

        if (len == 18) {
            return;           /* no longer need to extend */
        }

        /* try appending 0 and 1 */
        for (int bit = 0; bit < 2; ++bit) {
            uint32_t nb = (bits << 1) | bit;
            unsigned nl = len + 1;
            bool bad = false;

            /* check for a square ending at the new position
               for all block lengths L > 1 */
            for (unsigned L = 2; L <= nl / 2; ++L) {
                unsigned maskL = (1u << L) - 1;
                uint32_t last2 = nb & ((1u << (2 * L)) - 1);
                if ((last2 >> L) == (last2 & maskL)) {
                    bad = true;
                    break;
                }
            }

            if (!bad) {
                dfs(nb, nl, cnt);
            }
        }
    }
}

/* initialise the pre_cnt table if it hasn't been done yet */
static void init_precomputed(void)
{
    if (!precomputed) {
        precomputed = true;
        dfs(0u, 0, pre_cnt);
    }
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (n >= 19) {
        return (seq_t)0;
    }
    init_precomputed();
    return pre_cnt[(size_t)n];
}
