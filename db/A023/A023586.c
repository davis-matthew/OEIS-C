/*
 * A023586  Exponent of least prime factor in prime factorization of
 *          2*prime(n)-1.
 * https://oeis.org/A023586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exponent of the least prime factor in the factorization
 *            of 2*prime(n)-1
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* get the n-th prime */
    uint32_t p = get_prime(n);

    /* compute m = 2*p - 1 (always odd) */
    seq_t m = (seq_t)2 * (seq_t)p - 1;

    /* find the least prime factor l of m */
    seq_t l = 0;
    seq_t lim = floor_sqrt(m);

    /* check odd divisors from 3 upwards */
    for (seq_t d = 3; d <= lim; d += 2) {
        if (m % d == 0) {
            l = d;
            break;
        }
    }
    if (l == 0) l = m;          /* m itself is prime */

    /* count how many times l divides m */
    seq_t tmp = m;
    seq_t l_seq = l;
    int exp = 0;
    while (tmp % l_seq == 0) {
        tmp /= l_seq;
        exp++;
    }

    return (seq_t)exp;
}
