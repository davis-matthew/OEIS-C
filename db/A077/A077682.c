/*
 * A077682  Squarefree numbers beginning with 6.
 * https://oeis.org/A077682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 4049, u32 n <= 6761, u64 n <= 6761,
 *            u128 n <= 6761, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the n­th square‑free integer that begins with the digit 6.
   The function iterates through the natural numbers, skips those whose
   first decimal digit is not 6, and counts only the square‑free ones
   until the desired n‑th term is reached.  */
#include <stdbool.h>
#include "../utils/is_squarefree.h"

static inline unsigned char first_digit(seq_t x) {
    while (x >= 10) x /= 10;
    return (unsigned char)x;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* the offset is 1; this case never occurs */
    idx_t found = 0;
    seq_t cand   = 5;              /* start before the first candidate */

    while (1) {
        cand++;                    /* next integer */
        if (first_digit(cand) != 6) continue;
        if (is_squarefree(cand)) {
            found++;
            if (found == n) return cand;
        }
    }
}
