/*
 * A084681  Order of 10 modulo 9n [i.e., least m such that 10^m = 1 (mod 9n)]
 *          or 0 when no such number exists.
 * https://oeis.org/A084681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m>0 such that 10^m ≡ 1 (mod 9*n); if no such m
 *            exists (i.e., gcd(10,9*n)>1) a(n)=0
 * Range:     u8 n <= 28, u16 n <= 5388, u32 n <= 8908, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, gcd_seq_t, mod_pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helpers */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd_seq_t.h"
#include "../utils/calc_phi.h"
#include "../utils/mod_pow_seq.h"

seq_t a(idx_t n) {
    /* Compute m = 9*n (in seq_t) */
    seq_t m = (seq_t)n * 9;

    /* If 10 and m are not coprime, order does not exist */
    if (gcd_seq_t(m, (seq_t)10) != (seq_t)1)
        return (seq_t)0;

    /* φ(m) */
    seq_t phi_seq = calc_phi(m);
    /* Assume φ(m) fits in uint64_t for n up to ~10^5 (as in tests) */
    uint64_t order = (uint64_t)phi_seq;
    if (order == 0) return (seq_t)0; /* defensive */

    /* Find distinct prime factors of order */
    uint64_t tmp = order;
    uint64_t primes[64];
    size_t pcnt = 0;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            primes[pcnt++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) primes[pcnt++] = tmp;

    /* Reduce order by each prime factor as long as 10^(order/p) ≡ 1 (mod m) */
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        while (order % p == 0) {
            uint64_t candidate = order / p;
            seq_t res = mod_pow_seq((seq_t)10, candidate, m);
            if (res == (seq_t)1)
                order = candidate;
            else
                break;
        }
    }

    return (seq_t)order;
}
