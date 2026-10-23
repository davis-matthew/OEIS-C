/*
 * A125636  Smallest odd prime base q such that p^2 divides q^(p-1) - 1, where
 *          p = prime(n).
 * https://oeis.org/A125636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 196, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest odd prime q such that p^2 divides q^(p-1) – 1, where p is the nth prime. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    static uint64_t *prime_list = NULL;
    static size_t prime_count = 0;

    if (!prime_list) {
        /* Pre‑compute primes up to a safe bound for n ≤ 10 000 */
        const idx_t LIMIT = 20000000;           /* 20 million, > 10 thousandth prime */
        build_primes_upto(LIMIT, &prime_list, &prime_count);
    }

    if (n == 0 || (idx_t)(n - 1) >= prime_count)
        return 0;                              /* out of range – should not occur */

    uint64_t p = prime_list[n - 1];              /* nth prime (1‑based) */
    seq_t mod = (seq_t)p * (seq_t)p;             /* p^2 */

    /* Search for the smallest odd prime q satisfying q^(p-1) ≡ 1 (mod p^2) */
    for (size_t i = 1; i < prime_count; ++i) {   /* prime_list[0] = 2, start at 3 */
        uint64_t q = prime_list[i];
        if ((q & 1ULL) == 0) continue;          /* skip even 2 */
        if (mod_pow((seq_t)q, (uint64_t)(p - 1), mod) == 1)
            return (seq_t)q;
    }

    return 0; /* should never be reached for the intended input range */
}
