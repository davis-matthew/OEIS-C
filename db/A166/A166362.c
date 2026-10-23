/*
 * A166362  a(n) = phi(nonprime(n)).
 * https://oeis.org/A166362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = phi(nonprime(n))
 * Range:     u8 n <= 226, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* nonprime(n) is the nth composite integer including 1.  */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/phi.h"

seq_t a(idx_t n) {
    idx_t num = 1;            /* start with 1, the first nonprime */
    idx_t count = 0;          /* how many nonprimes seen so far */

    while (true) {
        if (!is_prime(num)) { /* 1 and every composite number */
            ++count;
            if (count == n) {
                return phi(num);
            }
        }
        ++num;
    }

    /* unreachable, but required by the compiler */
    return (seq_t)0;
}
