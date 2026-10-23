/*
 * A104332  Primes which are the reverse concatenation of two consecutive odd
 *          numbers.
 * https://oeis.org/A104332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = nth prime of the form concat(k+2, k) with k odd (i.e. a
 *            prime that is the reverse concatenation of two consecutive odd
 *            numbers)
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   digit_len, is_prime_seq, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    idx_t      count = 0;
    seq_t      k = 1;          /* smallest odd number */

    for (;;) {
        seq_t k2 = k + 2;                         /* next odd */
        idx_t d   = digit_len(k);                 /* digits of k      */
        seq_t p   = k2 * pow10(d) + k;             /* concat(k+2, k)  */

        if (is_prime_seq(p)) {
            count++;
            if (count == n)
                return p;
        }

        k += 2; /* next odd */
    }
}
