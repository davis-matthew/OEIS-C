/*
 * A040086  Primes p such that x^3 = 20 has no solution mod p.
 * https://oeis.org/A040086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = nth prime p such that x^3 ≡ 20 (mod p) has no solution.
   Primes 2, 3 and 5 are excluded.  For any other prime p,
   the congruence has a solution iff p ≡ 1 (mod 3) and
   20^((p − 1) / 3) ≡ 1 (mod p).  Thus a(n) lists the primes
   for which p ≡ 1 (mod 3) and 20^((p − 1) / 3) ≠ 1 (mod p). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

/* Integer square root of a 64‑bit unsigned integer (floor). */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = 0;
    uint64_t bit = 1ULL << 62;          /* 2^(2*31) <= sqrt(2^64-1) */
    while (bit != 0) {
        if (bit <= x - r) {              /* avoid overflow in r+bit      */
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* ----- memoised storage ------------------------------------ */
    static bool initialized = false;
    static uint64_t *prime_list = NULL;
    static idx_t prime_cnt = 0, prime_cap = 0;
    static seq_t *seq = NULL;
    static idx_t seq_cnt = 0, seq_cap = 0;
    static uint64_t next_candidate = 5; /* start after the primes 2,3,5 */

    if (!initialized) {
        prime_cap = 64;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        /* seed the list of known primes for trial division */
        prime_list[prime_cnt++] = 2;
        prime_list[prime_cnt++] = 3;
        prime_list[prime_cnt++] = 5;   /* 5 is needed for correct division */
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        initialized = true;
    }

    /* ----- generate primes until we have n results ------------- */
    while (seq_cnt < n) {
        uint64_t p = next_candidate;
        next_candidate += 2;          /* only odd candidates */

        /* primality test by trial division using known primes */
        bool is_prime = true;
        uint64_t limit = isqrt_u64(p);
        for (idx_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
            if (p % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (!is_prime)
            continue;

        /* store the new prime for future tests */
        if (prime_cnt == prime_cap) {
            prime_cap *= 2;
            prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
        }
        prime_list[prime_cnt++] = p;

        /* skip the primes 2, 3 and 5 */
        if (p == 2 || p == 3 || p == 5)
            continue;

        /* only primes congruent to 1 mod 3 can give non‑trivial results */
        if (p % 3 != 1)
            continue;

        /* exponent e = (p-1)/3 */
        uint64_t e = (p - 1) / 3;

        /* test whether 20^e ≡ 1 (mod p) */
        seq_t pow = mod_pow((seq_t)20, e, (seq_t)p);
        if (pow != (seq_t)1) {
            /* p has no solution of x^3 ≡ 20 (mod p) */
            if (seq_cnt == seq_cap) {
                seq_cap *= 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_cnt++] = (seq_t)p;
        }
    }

    return seq[n - 1];
}
