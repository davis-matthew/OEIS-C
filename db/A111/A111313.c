/*
 * A111313  Character codes of the string "The On-Line Encyclopedia of Integer
 *          Sequences".
 * https://oeis.org/A111313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, word, dumb
 * Formula:   a(n) = ASCII code of the nth character of the literal string
 *            "The On-Line Encyclopedia of Integer Sequences"
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The fixed string whose character codes form the sequence */
    static const char str[] =
        "The On-Line Encyclopedia of Integer Sequences";

    /* Length of the string (excluding the terminating NUL) */
    const idx_t len = sizeof(str) - 1U;

    /* Index out of bounds → a value of zero (defined behaviour) */
    if (n == 0 || n > len)
        return (seq_t)0U;

    /* Return the character code, promoted to seq_t safely */
    return (seq_t)(unsigned char)str[n - 1U];
}
