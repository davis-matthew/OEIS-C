/*
 * A066580  Let N = 123456789123456789123456789..., with the digits from 1 to
 *          9 repeated indefinitely. Then a(n) is the n-digit number formed
 *          from the digits starting at the {n(n-1)/2 +1}-th position of N,
 *          read backwards if n is even.
 * https://oeis.org/A066580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n‑digit number formed from the digits starting at
 *            position n(n‑1)/2 + 1 in the infinite repetition of 123456789,
 *            read forward if n is odd and backward if n is even
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the nth term of A066580 */
seq_t a(idx_t n)
{
    /* starting position in the infinite digit sequence */
    idx_t s = n * (n - 1) / 2 + 1;

    seq_t value = 0;

    if (n & 1) {               /* odd: read forward */
        for (idx_t i = 0; i < n; ++i) {
            idx_t pos = s + i;
            /* digit at position pos is ((pos-1) % 9) + 1 */
            uint64_t d = ((pos - 1) % 9) + 1;
            value = value * 10 + (seq_t)d;
        }
    } else {                    /* even: read backward */
        for (idx_t i = n; i > 0; --i) {
            idx_t pos = s + i - 1;
            uint64_t d = ((pos - 1) % 9) + 1;
            value = value * 10 + (seq_t)d;
        }
    }

    return value;
}
