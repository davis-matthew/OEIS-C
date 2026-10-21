/*
 * A230902  Positive numbers such that half of the set of divisors are of the
 *          form x^2 + x*y + y^2 (A003136) and half not (A034020).
 * https://oeis.org/A230902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   factor_u64, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A230902.  N is in the sequence iff among its divisors exactly
 * half are of the form x^2+xy+y^2 (A003136).  Equivalently, in the
 * prime factorisation, let the primes p≡2 (mod 3) have exponents ei.
 * Then the number is in the sequence exactly when  ∏(ei+1) is even and
 *  ∏(⌊ei/2⌋+1) = (∏(ei+1))/2. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_realloc.h"

static seq_t *seq_cache = NULL;
static size_t seq_cap   = 0;
static size_t seq_len   = 0;
static uint64_t next_scan = 1;

static void ensure_cache(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 256;
    while (new_cap < need) new_cap <<= 1;
    seq_cache = seq_realloc(seq_cache, seq_cap, new_cap);
    seq_cap   = new_cap;
}

static bool is_half_rep(uint64_t m)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;
    factor_u64(m, primes, exps, &cnt);

    seq_t B1 = 1;
    seq_t B2 = 1;

    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] % 3 == 2) {
            B1 *= (seq_t)(exps[i] + 1);
            B2 *= (seq_t)(exps[i] / 2 + 1);
        }
    }
    return ((B1 & 1u) == 0) && (B2 * 2 == B1);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (seq_len < (size_t)n) {
        if (is_half_rep(next_scan)) {
            ensure_cache(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)next_scan;
        }
        ++next_scan;
    }
    return seq_cache[(size_t)n - 1];
}
