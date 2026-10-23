/*
 * A175963  Numbers n with property that both n and n^2 have at least two 1's.
 * https://oeis.org/A175963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   digit_frequencies, grow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: enumerate integers n whose decimal representation
   contains at least two '1's and whose square also contains
   at least two '1's. Returns the n-th such number. */
#include <stddef.h>
#include "../utils/digit_frequencies.h"
#include "../utils/seq_mul.h"
#include "../utils/grow_seq.h"

/* Function implementing OEIS sequence A175963 */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* index starts at 1 */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (cache_len < n) {
        seq_t candidate = (cache_len == 0) ? (seq_t)10 : cache[cache_len - 1];
        unsigned int freq1[10];
        unsigned int freq2[10];

        while (cache_len < n) {
            candidate++;

            /* count '1's in candidate */
            digit_frequencies(candidate, freq1);
            if (freq1[1] < 2) continue;

            /* compute square and count '1's in it */
            seq_t sq = seq_mul(candidate, candidate);
            digit_frequencies(sq, freq2);
            if (freq2[1] < 2) continue;

            /* store candidate */
            if (cache_len == cache_cap) {
                grow_seq(&cache, &cache_cap, cache_len + 1);
            }
            cache[cache_len++] = candidate;
        }
    }

    return cache[n - 1];
}
