/*
 * A091067  Numbers whose odd part is of the form 4*k+3.
 * https://oeis.org/A091067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = the nth integer whose odd part is congruent to 3 (mod 4)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Off‑set 1: a(1) = 3 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) /* undefined for n<1, return 0 as a guard value */
        return 0;

    idx_t candidate = 1;   /* start just before the first valid number */
    idx_t found     = 0;   /* how many valid numbers we have seen */

    while (found < n) {
        candidate++;          /* next integer to test */

        /* strip powers of two to obtain the odd part */
        idx_t x = candidate;
        while ((x & 1) == 0)
            x >>= 1;

        /* odd part ≡ 3 (mod 4) → this is a term of the sequence */
        if ((x & 3) == 3)
            ++found;
    }

    /* cast back to seq_t once the correct term is identified */
    return (seq_t)candidate;
}
