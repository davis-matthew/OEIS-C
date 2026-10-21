/*
 * A056490  Number of periodic palindromes using exactly four different
 *          symbols.
 * https://oeis.org/A056490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12 * (S2(floor((n+1)/2),4) + S2(ceil((n+1)/2),4))
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   S2_four
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where S2(m,4) is the Stirling number of the second kind, 4‑th column. */

#include <stdbool.h>
#include "../utils/S2_four.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                 /* safety for n < 1 */
    }

    idx_t total = n + 1;          /* (n+1) */
    idx_t m1 = total >> 1;        /* floor((n+1)/2) */
    idx_t m2 = total - m1;        /* ceil((n+1)/2)  */

    seq_t s1 = S2_four(m1);
    seq_t s2 = S2_four(m2);

    return (seq_t)12 * (s1 + s2);
}
