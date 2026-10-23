/*
 * A113574  a(n) is the least n-digit number whose k-th digit is prime if k is
 *          prime, composite if k is composite, and 1 if k=1.
 * https://oeis.org/A113574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1)=1; a(n) = a(n-1)||2 if n is prime, else a(n-1)||4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (|| denotes decimal concatenation) */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t k = 1; k <= n; ++k) {
        val = val * 10;                    /* shift left one decimal digit */
        if (k == 1) {
            val += 1;                      /* 1st digit is 1 */
        } else if (is_prime((uint64_t)k)) {
            val += 2;                      /* prime position: digit 2 */
        } else {
            val += 4;                      /* composite position: digit 4 */
        }
    }
    return val;
}
