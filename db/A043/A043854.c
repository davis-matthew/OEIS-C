/*
 * A043854  Number of runs in the base 4 representation of n is congruent to 4
 *          mod 8.
 * https://oeis.org/A043854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer whose number of runs in base 4 is
 *            congruent to 4 modulo 8
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return true if seq_t x has a number of runs in its base‑4 representation
   congruent to 4 modulo 8. */
static inline bool is_good(seq_t x) {
    seq_t y = x;
    seq_t prev = 0;
    unsigned long long runs = 0;
    bool first = true;

    /* Handle the unlikely case x == 0; not needed for this sequence but makes
       the function coherent. */
    if (y == 0) {
        runs = 1;
    } else {
        while (y > 0) {
            seq_t d = y % 4;   /* current base‑4 digit */
            if (first) {
                runs = 1;
                first = false;
            } else {
                if (d != prev) runs++;
            }
            prev = d;
            y /= 4;
        }
    }
    return ((runs & 7ULL) == 4ULL);  /* runs mod 8 == 4 */
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n) {
    idx_t found = 0;
    seq_t num = 1;                 /* candidate numbers start at 1 */
    while (1) {
        if (is_good(num)) {
            found++;
            if (found == n) return num;
        }
        num++;                      /* advance to the next integer */
    }
}
