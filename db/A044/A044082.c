/*
 * A044082  Numbers n such that string 1,0 occurs in the base 5 representation
 *          of n but not of n-1.
 * https://oeis.org/A044082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has10_seq5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A044082: numbers n whose base‑5 representation contains the
 * substring "10" but that of n-1 does not.
 * The function performs a simple brute‑force search.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has10_seq5.h"

/* Return the nth term of the sequence (offset = 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* The offset is 1; this value is unused. */

    idx_t count = 0;
    seq_t cand = (seq_t)1;

    for (;; ++cand) {
        if (has10_seq5(cand)) {
            seq_t prev = cand - ((seq_t)1);
            if (!has10_seq5(prev)) {
                ++count;
                if (count == n)
                    return cand;
            }
        }
    }
}
