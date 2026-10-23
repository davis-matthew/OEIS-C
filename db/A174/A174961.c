/*
 * A174961  Number of non-unitary divisors of the n-th nonsquarefree number.
 * https://oeis.org/A174961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A174961
    Formula: a(n) = tau(A013929(n)) – 2^ω(A013929(n))
    where τ(n) is the divisor count and ω(n) is the number of distinct
    prime factors.  A013929(n) is the nth nonsquarefree integer.  The
    implementation enumerates nonsquarefree numbers on demand and
    computes the answer by trial division using a pre‑computed prime list. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Cached prime list used for trial division. */
static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;

/* Ensure that a sufficient list of primes is ready. */
static void init_primes(void)
{
    if (prime_list != NULL) return;
    /* 20000 comfortably exceeds sqrt(≈33000) – the value of the 10 000th
       nonsquarefree number. */
    const idx_t LIMIT = 20000;
    build_primes_upto(LIMIT, (uint64_t **)&prime_list, &prime_cnt);
}

/* Return the a(n) sequence value for any n ≥ 1.  The function works
   correctly for all n ≥ 1 and is independent of the order of calls. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined – return 0 safely */

    init_primes();                 /* prime list ready for factorisation */

    idx_t seen = 0;                /* number of nonsquarefrees processed */
    uint64_t cand = 4;              /* first nonsquarefree integer */

    while (true) {
        uint64_t t = cand;
        seq_t tau = 1;             /* divisor count ∏(e_i + 1) */
        unsigned omega = 0;        /* count of distinct prime factors */
        bool not_sq = false;       /* true if any exponent > 1 */

        /* Trial division using the pre‑computed primes */
        for (size_t pi = 0; pi < prime_cnt && prime_list[pi] * prime_list[pi] <= t; ++pi) {
            uint64_t p = prime_list[pi];
            unsigned e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
                if (e > 1) not_sq = true;
            }
            if (e > 0) {
                tau *= (seq_t)(e + 1);
                ++omega;
            }
        }

        /* Remaining prime factor, if any */
        if (t > 1) {
            tau *= (seq_t)2;
            ++omega;
        }

        if (not_sq) {
            /* Compute 2^omega */
            seq_t two_pow = 1;
            for (unsigned k = 0; k < omega; ++k) two_pow *= (seq_t)2;

            seq_t result = tau - two_pow;
            ++seen;
            if (seen == n) return result;
        }

        ++cand;
    }
}
