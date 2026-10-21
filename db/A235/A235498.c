/*
 * A235498  For k in {2,3,...,9} define a sequence as follows: a(0)=0; for
 *          n>=0, a(n+1)=a(n)+1, unless a(n) ends in k, in which case a(n+1)
 *          is obtained by replacing the last digit of a(n) with the digit(s)
 *          of k^2. This is k(2).
 * https://oeis.org/A235498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*floor(n/9) + [0,1,2,4,5,6,7,8,9][n mod 9] (Sequence of
 *            nonnegative integers whose decimal representation does not end
 *            in 3.)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Predefined digits of a full 10-number cycle with the
       value 3 removed. */
    static const seq_t digits[9] = {0, 1, 2, 4, 5, 6, 7, 8, 9};

    idx_t q = n / 9;            /* Number of full 10‑digit cycles. */
    idx_t r = n % 9;            /* Position inside the current cycle. */

    /* Compute 10 * q using seq_t arithmetic to avoid overflow
       for arbitrary‑precision builds. */
    seq_t base = (seq_t)q * (seq_t)10;

    /* r < 9, so cast to unsigned for array indexing. */
    seq_t result = base + digits[(unsigned)r];

    return result;
}
