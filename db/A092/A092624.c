/*
 * A092624  Numbers with exactly two prime digits.
 * https://oeis.org/A092624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search for numbers with exactly two prime decimal digits
 *            (2,3,5,7)
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/count_prime_digits.h"

/* seq_t a(idx_t n)
   ---  nth term of A092624 (offset 1) ------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined in offset 1, but return 0 for safety */
        return 0;
    idx_t found = 0;
    seq_t num = 1;
    while (found < n) {
        if (count_prime_digits(num) == 2) {
            found++;
            if (found == n)
                return num;
        }
        num++;
    }
    return 0; /* unreachable */
}
