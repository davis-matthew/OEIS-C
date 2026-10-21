/*
 * A230329  Prime(prime(2*n)) - 2*prime(n).
 * https://oeis.org/A230329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(prime(2*n)) - 2*prime(n)
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* compute prime(2*n) */
    idx_t two_n = 2 * n;
    uint32_t p1 = get_prime(two_n);
    /* compute prime(prime(2*n)) */
    uint32_t p2 = get_prime(p1);
    /* compute prime(n) */
    uint32_t p3 = get_prime(n);
    /* return the difference */
    return (seq_t)p2 - ((seq_t)2 * (seq_t)p3);
}
