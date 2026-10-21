/*
 * A081339  Numbers n such that sigma(n^2) modulo 4 = 1.
 * https://oeis.org/A081339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A081339: numbers n such that σ(n²) ≡ 1 (mod 4).
 *  The function a(n) returns the n‑th such integer.
 *  It tests successive candidates and stores those that satisfy the
 *  property.  The condition reduces to counting how many factors
 *  contribute 3 modulo 4 (each odd exponent of a prime ≡ 1 (mod 4)
 *  and a factor 2).  Even count of such factors ⇒ σ(n²) ≡ 1 (mod 4).
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static bool qualifies(idx_t cand)
{
    /* Count the number of 3‑mod‑4 factors modulo 2 */
    unsigned parity = (cand & 1) ? 0 : 1;    /* 2 is present iff cand even */

    /* factorise cand */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t count = 0;
    factor_u64((uint64_t)cand, primes, exps, &count);

    for (size_t i = 0; i < count; ++i) {
        uint64_t p = primes[i];
        if (p == 2) continue;                 /* already accounted for */
        if ((p & 3u) == 1u && (exps[i] & 1u))
            parity ^= 1;                     /* odd exponent of p≡1 (mod 4) */
    }
    return parity == 0;   /* even number of 3‑mod‑4 factors */
}

seq_t a(idx_t n)
{
    /* static storage for the sequence */
    static seq_t *seq        = NULL;
    static size_t seq_cap    = 0;
    static size_t seq_len    = 0;
    static idx_t next_candidate = 1;

    while (seq_len < n) {
        if (qualifies(next_candidate)) {
            if (seq_len + 1 > seq_cap) {
                size_t newcap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
                if (!tmp) abort();           /* allocation failed */
                seq = tmp;
                seq_cap = newcap;
            }
            seq[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return seq[n - 1];
}
