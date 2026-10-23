/*
 * A137305  Write n in base 3, change twos in ones and ones in twos, reverse.
 * https://oeis.org/A137305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the number obtained by writing n in base 3, swapping 1↔2
 *            (leaving 0 unchanged), then reversing the digits
 * Range:     u8 n <= 243, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == 0) return 0;

    /* Max number of base‑3 digits for a 64‑bit number is 40,
       so an array of 64 is safely large for any idx_t. */
    seq_t digits[64];
    size_t len = 0;

    /* Convert n to base 3, swap digits on the fly, and store
       the swapped digits in order of extraction (least significant first). */
    while (n != 0) {
        uint64_t d = n % 3;          /* idx_t modulo fits in uint64_t            */
        n /= 3;
        seq_t swapped;
        if (d == 1) swapped = (seq_t)2;
        else if (d == 2) swapped = (seq_t)1;
        else swapped = (seq_t)0;
        digits[len++] = swapped;
    }

    /* Reconstruct the number by interpreting the stored digits
       in the same order (least significant first), which yields
       the reversed swapped representation. */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        result = result * ((seq_t)3) + digits[i];
    }
    return result;
}
