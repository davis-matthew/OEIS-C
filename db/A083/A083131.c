/*
 * A083131  Number the letters of English alphabet from 1 to 26; sequence
 *          specifies capital letters with left-right symmetry.
 * https://oeis.org/A083131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn, fini, full
 * Formula:   a(n) = the position of the nth capital letter of the English *
 *            alphabet that is left‑right symmetric. The symmetric letters are
 *            * A(1), H(8), I(9), M(13), O(15), T(20), U(21), V(22), W(23),
 *            X(24), Y(25)
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Bitmask of symmetric letters: bit 0 for letter 1 (A), bit 7 for 8 (H), etc. */
    static const uint32_t sym_mask =
          (1u << (1-1)) |  /* A */
          (1u << (8-1)) |  /* H */
          (1u << (9-1)) |  /* I */
          (1u << (13-1)) | /* M */
          (1u << (15-1)) | /* O */
          (1u << (20-1)) | /* T */
          (1u << (21-1)) | /* U */
          (1u << (22-1)) | /* V */
          (1u << (23-1)) | /* W */
          (1u << (24-1)) | /* X */
          (1u << (25-1));  /* Y */

    idx_t count = 0;
    uint32_t mask = sym_mask;
    for (idx_t pos = 1; pos <= 26; ++pos)
    {
        if (mask & 1u)
        {
            ++count;
            if (count == n)
                return (seq_t)pos;
        }
        mask >>= 1;
    }
    /* If n is larger than the number of symmetric letters, return 0. */
    return (seq_t)0;
}
