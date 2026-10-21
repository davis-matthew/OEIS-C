/*
 * A083132  Number the letters of English alphabet from 1 to 26; sequence
 *          specifies capital letters with up-down symmetry.
 * https://oeis.org/A083132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn, fini, full
 * Formula:   a(n) = 26*⌊(n-1)/9⌋ + r[(n-1) mod 9], where r =
 *            {2,3,4,5,8,9,11,15,24} (indices of letters with up‑down
 *            symmetry)
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    static const seq_t r[9] = {2,3,4,5,8,9,11,15,24};
    idx_t block = (n - 1) / 9;      /* number of full cycles of 26 */
    idx_t pos   = (n - 1) % 9;      /* position within current cycle */
    seq_t result = (seq_t)26 * (seq_t)block + r[pos];
    return result;
}
