/*
 * A032723  Numbers k such that k prefixed by '6' and followed by '7' is a
 *          prime.
 * https://oeis.org/A032723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 28, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is the nth integer k such that the decimal integer
   formed by prefixing 6 and suffixing 7 – i.e. 6k7 – is prime.  The
   function searches k = 0,1,2,… and tests each corresponding
   number for primality. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* offset is 1 */

    idx_t found = 0;                   /* how many qualifying k we have seen */
    seq_t k = 0;                       /* current candidate k */
    seq_t power10d = 10;                /* 10^d ; d is number of digits of k */

    while (true) {
        /* 6k7 = 60*10^d + k*10 + 7   (with d = digits(k)) */
        seq_t cur = 60 * power10d + 7;
        cur += k * 10;

        if (is_prime_big(cur)) {
            ++found;
            if (found == n)
                return k;
        }

        ++k;

        /* When k reaches the next digit length, increase power10d */
        if (k == power10d) {
            power10d *= 10;
        }
    }
}
