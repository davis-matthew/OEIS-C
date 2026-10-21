/*
 * A211027  Triangle of binary numbers >= 1 with no initial repeats.
 * https://oeis.org/A211027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 297, u64 n <= 8959,
 *            u128 n <= 8959, big any n
 * Helpers:   no_initial_repeat
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A211027: triangle of binary numbers ≥1 with no
   initial repeats.  Each term is output as the decimal integer whose
   digit string is the binary word.  For example the word 101 is
   returned as 101 (not 5). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/no_initial_repeat.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t target   = n;
    idx_t cumulative = 0;

    for (int len = 1; ; ++len) {
        /* The number of words of this length is 2^(len-1). */
        uint64_t end = (len <= 63) ? ((uint64_t)1 << (len - 1)) : 0;
        idx_t rank = 0;

        for (uint64_t mask = 0; mask < end; ++mask) {
            seq_t word = ((seq_t)1 << (len - 1)) | (seq_t)mask;
            if (!no_initial_repeat(word, len))
                continue;

            ++rank;
            if (cumulative + rank == target) {
                /* Compute the decimal value of the binary word. */
                seq_t dec = 1;          /* leading '1' */
                for (int pos = len - 2; pos >= 0; --pos) {
                    int bit = (int)((mask >> pos) & 1);
                    dec = dec * 10 + bit;
                }
                return dec;
            }
        }
        cumulative += rank;
    }
    return 0; /* unreachable for valid n */
}
