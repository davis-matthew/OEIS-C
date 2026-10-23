/*
 * A181570  Primes in A050798.
 * https://oeis.org/A181570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that (p^2 + 1) is representable * as
 *            the sum of two non‑zero squares in exactly two unordered ways
 * Range:     u8 n <= 2, u16 n <= 26, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   append_candidate, factor_u64, is_prime, isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/isqrt_seq.h"
#include "../utils/append_candidate.h"

#ifndef seq_t
/* seq_t is defined by the build system */
#endif
#ifndef idx_t
/* idx_t is defined by the build system */
#endif

static seq_t *term_cache = NULL;
static size_t term_len = 0;
static size_t term_cap = 0;
static idx_t candidate = 2; /* start testing from 2 */

/* Count the number R of unordered positive representations of x as
 * a^2 + b^2, a >= b > 0. */
static seq_t count_representations(seq_t x) {
    if (x == 0) return 0;
    uint64_t p[64];
    uint64_t e[64];
    size_t cnt = 0;
    factor_u64((uint64_t)x, p, e, &cnt);

    /* Check for prime factors congruent to 3 mod 4 with odd exponent */
    for (size_t i = 0; i < cnt; ++i) {
        if ((p[i] % 4 == 3) && (e[i] & 1)) return 0;
    }

    /* Compute product of (exp+1) over primes ≡ 1 (mod 4) */
    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (p[i] % 4 == 1) {
            prod *= (seq_t)(e[i] + 1);
        }
    }

    seq_t r2 = prod * 4;                 /* r2 = number of all integer solutions */
    bool twice_square = false;
    if ((x & 1) == 0) {
        seq_t half = x / 2;
        seq_t sq = isqrt_seq(half);
        if (sq * sq == half) twice_square = true;
    }

    if (twice_square) {
        /* Subtract the 4 solutions arising from the equal pair,
         * then divide by 8, finally add the single equal pair. */
        return (r2 - 4) / 8 + 1;
    } else {
        return r2 / 8;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for offset 1, but safety */

    /* Grow cache until we have n terms */
    while (term_len < n) {
        if (is_prime(candidate)) {
            /* Check if candidate satisfies the property */
            seq_t pcand = (seq_t)candidate;
            seq_t m = pcand * pcand + 1;
            if (count_representations(m) == 2) {
                append_candidate(&term_cache, &term_len, &term_cap, pcand);
            }
        }
        ++candidate;
    }

    return term_cache[n - 1];
}
