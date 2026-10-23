/*
 * A154965  Composites n such that concatenation of n and n-th prime is prime.
 * https://oeis.org/A154965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 15, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_count, get_prime, is_prime, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth composite m such that the concatenation of m and the m‑th
 * prime is itself prime. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"          /* bool is_prime(idx_t x); */
#include "../utils/get_prime.h"         /* uint32_t get_prime(idx_t k); */
#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t n); */
#include "../utils/pow10.h"             /* seq_t pow10(idx_t e); */
#include "../utils/digit_count.h"       /* uint64_t digit_count(uint64_t n); */

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (idx_t m = 4; ; ++m) {
        if (is_prime(m))           /* skip primes */
            continue;

        uint32_t p32 = get_prime(m);        /* the m‑th prime (uint32_t) */
        uint64_t p = (uint64_t)p32;          /* for digit count */

        uint64_t d = digit_count(p);       /* number of decimal digits of p */
        seq_t pow = pow10((idx_t)d);        /* 10^d   (seq_t) */
        seq_t c = (seq_t)m * pow + (seq_t)p;/* concat(m,p) */

        if (is_prime_big(c)) {
            ++found;
            if (found == n)
                return (seq_t)m;
        }
    }
}
