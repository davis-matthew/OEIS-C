/*
 * A078498  Let q(n) be the prime defined in A078497; sequence gives
 *          (q(n)-prime(n))/6.
 * https://oeis.org/A078498
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (min{p : p > prime(n), p and 2*prime(n)-p are primes} -
 *            prime(n)) / 6
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) equals the smallest k≥1 such that prime(n)–6k and prime(n)+6k are primes; return k. */

#include "../utils/get_prime.h"
#include "../utils/is_prime.h"
#include <stdint.h>

seq_t a(idx_t n) {
    /*  The sequence is defined only for n ≥ 5.  Return 0 for smaller indices. */
    if (n < 5) return 0;

    /* Fetch the n-th prime.  get_prime returns a uint32_t; cast to idx_t. */
    idx_t pn = (idx_t)get_prime(n);

    idx_t k = 1;
    for (;;) {
        idx_t lower = pn - 6 * k;
        idx_t upper = pn + 6 * k;

        /* lower must be ≥ 2 to be a prime; for n ≥ 5 this is always true for the first few k, 
           but we guard against underflow. */
        if (lower >= 2 && is_prime(lower) && is_prime(upper))
            return (seq_t)k;

        ++k;
    }
}
