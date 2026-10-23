/*
 * A039270  Numbers whose base-12 representation has the same number of 8's
 *          and 10's.
 * https://oeis.org/A039270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 187, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_equal_8_10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This function generates the numbers whose base‑12 representation
 * contains an equal number of digits 8 and 10 (digit value 10).
 * It searches integers sequentially and returns the n‑th such number.
 * (OEIS A039270)
 */

#include <stdbool.h>
#include "../utils/has_equal_8_10.h"

/* Return the n‑th term (n >= 1) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for out‑of‑bounds calls */

    idx_t found = 0;   /* number of qualifying values seen */
    seq_t val   = 0;   /* current integer to test */

    while (1) {
        if (has_equal_8_10(val)) {
            ++found;
            if (found == n) return val;
        }
        ++val;
    }
}
