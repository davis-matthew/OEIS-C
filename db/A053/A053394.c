/*
 * A053394  The full list of 3-Kaprekar numbers.
 * https://oeis.org/A053394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) = n such that ⌊n²/1000⌋ + (n² mod 1000) = n (3‑Kaprekar
 *            numbers in base 10). The sequence is finite; the largest term is
 *            1000
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t count = 0;
    idx_t x = 1;

    while (1) {
        unsigned __int128 sq = (unsigned __int128)x * x;
        idx_t left  = (idx_t)(sq / 1000);
        idx_t right = (idx_t)(sq % 1000);

        if (left + right == x) {
            ++count;
            if (count == n)
                return (seq_t)x;
        }

        ++x;
        /* Once x² > 1000·x + 999, no larger n can satisfy the
           Kaprekar condition, so we can stop searching. */
        if ((unsigned __int128)x * x > (unsigned __int128)1000 * x + 999)
            break;
    }

    return (seq_t)0;  /* n is beyond the known terms */
}
