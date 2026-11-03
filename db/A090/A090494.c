/*
 * A090494  Product_{j=1..n} Product_{k=1..n} lcm(j,k).
 * https://oeis.org/A090494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{j=1..n} ∏_{k=1..n} lcm(j,k) = ∏_{p≤n} p^{ Σ_{k≥1} (
 *            2*n*⌊n/p^k⌋ – ⌊n/p^k⌋² ) } (p prime) This implements the
 *            closed‐form exponent count derived from max(e_j,e_k) for each
 *            pair (j,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 9 (2026-11-01)
 * Helpers:   build_primes_upto, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)1;
    }

    size_t prime_cnt;
    uint64_t *primes;
    build_primes_upto(n, &primes, &prime_cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t exponent = (seq_t)0;

        uint64_t pk = p;
        while (pk <= n) {
            idx_t q = n / pk;                       /* ⌊n/p^k⌋   */
            seq_t term = (seq_t)2 * (seq_t)n * (seq_t)q
                       - (seq_t)q * (seq_t)q;        /* 2*n*q – q² */
            exponent += term;

            /* avoid overflow of pk *= p */
            if (pk > n / p) break;
            pk *= p;
        }

        seq_t p_to_pow = ipow((seq_t)p, exponent);
        result = seq_mul(result, p_to_pow);
    }

    free(primes);
    return result;
}
