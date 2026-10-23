/*
 * A043865  Numbers n such that number of runs in the base 4 representation of
 *          n is congruent to 7 mod 9.
 * https://oeis.org/A043865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A043865:
     Numbers n such that the number of runs in the base‑4 representation
     of n is congruent to 7 (mod 9).  Offset 1.
     The function scans the natural numbers in order and returns the nth
     qualifying value.  This is a direct search implementation.
*/

#include <stdbool.h>
#include <stddef.h>

static inline bool has_property(seq_t x)
{
    /* Count runs in base‑4 representation of x.  A run is a maximal block
       of equal consecutive digits.  Returns true if the run count ≡ 7 (mod 9). */
    seq_t tmp = x;
    unsigned char dig[64];
    size_t len = 0;

    if (tmp == 0) {
        dig[0] = 0;   /* representation "0" */
        len = 1;
    } else {
        while (tmp > 0) {
            dig[len++] = (unsigned char)(tmp % 4);
            tmp /= 4;
        }
    }

    size_t transitions = 0;
    for (size_t i = 0; i + 1 < len; ++i) {
        if (dig[i] != dig[i + 1])
            ++transitions;
    }
    /* runs = transitions + 1 */
    return ((transitions + 1) % 9) == 7;
}

seq_t a(idx_t n)
{
    /* Return the nth term of the sequence (1‑based). */
    seq_t value = 0;
    idx_t count = 0;

    while (1) {
        if (has_property(value)) {
            ++count;
            if (count == n) {
                return value;
            }
        }
        ++value;
    }
}
