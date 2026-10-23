/*
 * A102130  Primes of the form 8*n^2 + 4*n + 1.
 * https://oeis.org/A102130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth prime of the form 8*n^2 + 4*n + 1
 * Range:     u8 n <= 2, u16 n <= 30, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for NULL */
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    idx_t k = 1;          /* k corresponds to the n in the quadratic */
    idx_t count = 0;      /* number of such primes found so far */

    while (true) {
        seq_t k_s  = (seq_t)k;                          /* cast to seq_t for the arithmetic */
        seq_t val  = 8 * k_s * k_s + 4 * k_s + 1;      /* 8k^2 + 4k + 1 */

        if (is_prime_seq(val)) {
            count++;
            if (count == n) {                          /* desired nth prime */
                return val;
            }
        }
        k++;                                          /* test next candidate */
    }
}
