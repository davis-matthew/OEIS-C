/*
 * A102490  Numbers in base-16 representation that cannot be written with
 *          decimal digits.
 * https://oeis.org/A102490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 156, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_hex_letters
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102490: numbers whose hexadecimal representation contains at least one digit A‑F.
   Computation is done by scanning integers and counting those with hex digits ≥10. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_hex_letters.h"

seq_t a(idx_t n)
{
    idx_t count   = 0;   /* how many qualifying numbers seen */
    idx_t current = 0;   /* current integer being tested */

    while (count < n) {
        ++current;                      /* examine the next integer */
        if (has_hex_letters(current))
            ++count;                     /* it qualifies */
    }

    return (seq_t)current;               /* return the nth qualifying number */
}
