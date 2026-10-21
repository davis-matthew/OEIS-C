/*
 * A226960  Numbers n such that 1^n + 2^n + 3^n +...+ n^n == 2 (mod n).
 * https://oeis.org/A226960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   append_candidate, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: n satisfies 1^n + 2^n + … + n^n ≡ 2 (mod n).
 * Equivalent test (used in this implementation):
 *  For each distinct prime divisor p of n, if (p‑1) divides n,
 *  include n/p in a sum.  Then n is a term iff
 *  sum ≡ n‑2 (mod n)  (i.e. sum % n == n-2).
 * Thus we only need prime‐factorisation of n to decide.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/append_candidate.h"

static seq_t *seq_ints = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static idx_t next_candidate = 0;

/* Check whether m satisfies the congruence */
static bool term_candidate(idx_t m)
{
    if (m == 1)
        return true;          /* 1 always satisfies */

    uint64_t tmp = m;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(tmp, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        if (m % (p - 1) == 0) {
            sum += (seq_t)(m / p);
        }
    }
    seq_t r = sum % (seq_t)m;
    return r == (seq_t)(m - 2);
}

/* Return the n-th term of the sequence (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined, return 0 */

    while (seq_len < n) {
        ++next_candidate;
        if (term_candidate(next_candidate))
            append_candidate(&seq_ints, &seq_len, &seq_cap, (seq_t)next_candidate);
    }
    return seq_ints[n - 1];
}
