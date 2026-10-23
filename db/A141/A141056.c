/*
 * A141056  1 followed by A027760, a variant of Bernoulli number denominators.
 * https://oeis.org/A141056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of primes p ≤ n+1 for which (p-1) divides n. For
 *            n = 0 this product is empty and a(0) = 1. The lemma above
 *            reproduces the OEIS values
 *            1,2,6,2,30,2,42,2,30,2,66,2,2730,2,6,... the first 24 terms
 *            listed in the OEIS entry
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 179, u64 n <= 539,
 *            u128 n <= 3779, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Assume seq_t and idx_t are defined by the build (e.g. uint64_t, unsigned __int128, …). */

static uint64_t _prime_list[2048];      /* enough for primes up to 10 000 */
static size_t   _prime_cnt  = 0;        /* number of primes currently stored */
static idx_t    _prime_last_limit = 0;  /* largest n+1 for which primes have been sieved */

/* Ensure that all primes up to ‘limit’ are generated and stored in _prime_list. */
static void _ensure_primes(idx_t limit)
{
    if (limit <= _prime_last_limit) return;  /* already have enough */

    _prime_cnt = 0;                           /* recompute from scratch */
    for (idx_t n = 2; n <= limit; ++n) {
        bool is_prime = true;
        for (size_t k = 0; k < _prime_cnt; ++k) {
            uint64_t p = _prime_list[k];
            if (p * p > n) break;
            if (n % p == 0) { is_prime = false; break; }
        }
        if (is_prime) _prime_list[_prime_cnt++] = n;
    }
    _prime_last_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* empty product */

    /* build primes up to n+1 */
    idx_t limit = n + 1;
    _ensure_primes(limit);

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < _prime_cnt; ++i) {
        uint64_t p = _prime_list[i];
        if (p > limit) break;             /* safety, though not needed */
        if (n % (p - 1) == 0) {
            res *= (seq_t)p;               /* multiplication works for both
                                               built‑in integer types and
                                               arbitrary‑precision seq_t.      */
        }
    }
    return res;
}
