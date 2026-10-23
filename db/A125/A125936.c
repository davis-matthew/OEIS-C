/*
 * A125936  Sprague-Grundy values for octal game .7707.
 * https://oeis.org/A125936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sprague‑Grundy values for octal game .7707.
   Computed by mex of XOR of Grundy values of positions obtainable
   by removing 1, 2, or 4 tokens from a heap of size n. */

#include <stddef.h>
#include <stdint.h>

#define MAX_N      20000     /* maximum n we support (≥ 10000)    */
#define SEEN_SIZE  131072    /* table size used for mex operation  */

static seq_t   grundy[MAX_N + 1];   /* Grundy numbers, 0‑based          */
static idx_t   cache_size = 0;     /* largest n computed so far         */
static unsigned int seen[SEEN_SIZE];
static unsigned int tag_counter = 1; /* unique marker for each mex run   */

/* Compute a(n) for the octal game .7707 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n = 0; return 0 safely */
        return (seq_t)0;

    /* initialize base case on first call */
    if (cache_size == 0)
        grundy[0] = (seq_t)0;

    /* extend cache if necessary */
    if (n > cache_size) {
        for (idx_t m = cache_size + 1; m <= n; ++m) {
            unsigned int tag = ++tag_counter;   /* new timestamp for mex */

            /* legal removal sizes: 1, 2, 4 */
            const idx_t moves[] = {1, 2, 4};
            const size_t move_cnt = 3;

            for (size_t mi = 0; mi < move_cnt; ++mi) {
                idx_t k = moves[mi];
                if (m < k) continue;

                idx_t rem = m - k;                 /* remaining tokens */
                for (idx_t left = 0; left <= rem; ++left) {
                    idx_t right = rem - left;
                    unsigned int g =
                        ((unsigned int)grundy[(size_t)left]) ^
                        ((unsigned int)grundy[(size_t)right]);

                    if (g < SEEN_SIZE)
                        seen[g] = tag;           /* mark reachable Grundy */
                }
            }

            /* mex: smallest non‑negative integer not marked */
            unsigned int mex = 0;
            while (mex < SEEN_SIZE && seen[mex] == tag)
                ++mex;

            grundy[(size_t)m] = (seq_t)mex;
        }
        cache_size = n;
    }

    return grundy[(size_t)n];
}
