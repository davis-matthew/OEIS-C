/*
 * A032809  Numbers whose set of base-9 digits is {2,3}.
 * https://oeis.org/A032809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑9 digits are {2,3}, in ascending order
   (OEIS A032809).  The n‑th term is obtained by enumerating all
   base‑9 numbers whose digits are either 2 or 3.  The first 2ⁿ
   numbers with exactly n digits are handled together, and the
   offset within that block is interpreted as a binary counter
   with 0 ↦ 2 and 1 ↦ 3.  This yields the sequence in strictly
   increasing numeric order.  */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find the block (length) that contains the n-th number. */
    idx_t cur   = 0;      /* number of terms with length < L */
    idx_t block = 2;      /* size of the block of length L = 1  (2¹ )    */
    size_t L    = 1;      /* current length */

    /* Advance to the correct block.  We stop when adding the next
       block would exceed n. */
    while (cur + block < n) {
        cur += block;
        ++L;
        block <<= 1;       /* block = 2^L */
    }

    /* Offset of the desired number within the block (0‑based). */
    idx_t offset = n - cur - 1;

    /* Convert the offset to a base‑9 number using digits 2 or 3. */
    seq_t value = 0;
    for (int i = (int)L - 1; i >= 0; --i) {
        seq_t bit   = (offset >> i) & 1;
        seq_t digit = bit ? 3 : 2;
        value = value * 9 + digit;
    }

    return value;
}
