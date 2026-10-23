/*
 * A107426  Prime numbers p such that the sum of the terms of the continued
 *          fraction for prime(p)/p is prime.
 * https://oeis.org/A107426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime p such that the sum of the terms of the
 *            continued fraction of prime(p)/p is itself prime
 * Range:     u8 n <= 10, u16 n <= 362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* not used, but guard */

    idx_t found = 0;
    uint64_t idx = 1;               /* 1‑based index of the current prime p */

    while (1) {
        seq_t p_seq = get_prime(idx);                     /* the current prime p */
        uint64_t p = (uint64_t)p_seq;                     /* p fits in 64 bits for our range */

        seq_t q_seq = get_prime(p);                      /* the p‑th prime, i.e. prime(p) */
        uint64_t q = (uint64_t)q_seq;                    /* q fits in 64 bits */

        /* Continue fraction of q/p:  {a0; a1, a2, ...}  */
        uint64_t a0 = q / p;
        uint64_t sum = a0;
        uint64_t r = q % p;
        uint64_t p_local = p;    /* keep original p for the algorithm */

        while (r != 0) {
            uint64_t term = p_local / r;
            sum += term;
            uint64_t r2 = p_local % r;
            p_local = r;
            r = r2;
        }

        if (is_prime((uint64_t)sum)) {
            ++found;
            if (found == n)
                return p_seq;                    /* p is the required term */
        }

        ++idx;     /* move to next prime p */
    }
}
