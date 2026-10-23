/*
 * A154299  Primes of the form (1+...+m)/51 = A000217(m)/51.
 * https://oeis.org/A154299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find primes p such that p = (k*(k+1)/2)/51 for k=1..102.
   Return the nth such prime with offset 1; 0 if n is outside the range. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* static cache of the finite set of primes */
    static seq_t primes[4];
    static size_t primes_cnt = 0;

    /* initialise cache on first call */
    if (primes_cnt == 0) {
        size_t i = 0;
        for (size_t k = 1; k <= 102 && i < 4; ++k) {
            uint64_t tri = (uint64_t)k * (k + 1) / 2;   /* T_k */
            if (tri % 51 == 0) {
                uint64_t p64 = tri / 51;
                seq_t p = (seq_t)p64;
                if (is_prime_big(p)) {
                    primes[i++] = p;
                }
            }
        }
        primes_cnt = i;
    }

    /* return nth prime or 0 if n is out of range */
    if (n == 0) {
        return (seq_t)0;  /* offset is 1, so a(0) is undefined; return 0 */
    }
    if ((size_t)(n - 1) < primes_cnt) {
        return primes[n - 1];
    }
    return (seq_t)0;
}
