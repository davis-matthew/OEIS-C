/*
 * A084704  Smallest prime p > prime(n) such that (p + prime(n))/2 is prime.
 * https://oeis.org/A084704
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 6509, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A084704: a(n) = smallest prime p > prime(n) such that (p + prime(n))/2 is prime. */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/next_prime.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* defined only for n >= 2 (offset 2) */

    /* prime(n) */
    uint32_t pn = get_prime(n);         /* nth prime (uint32_t for n <= 10000) */

    /* start searching with the next prime after prime(n) */
    uint64_t p = next_prime(pn);        /* smallest prime > pn */

    /* find the first p such that the average is prime */
    while (!is_prime((idx_t)((p + pn) >> 1))) {
        p = next_prime(p);              /* next prime after current p */
    }

    return (seq_t)p;
}
