/*
 * A062664  Composite and every divisor (except for 1) contains the digit 2.
 * https://oeis.org/A062664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n‑th composite integer all of whose non‑trivial divisors
 *            contain digit 2 * The function searches integers, tests the
 *            property, and caches results so * calls in any order remain fast
 * Range:     u8 n <= 1, u16 n <= 981, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_digit_2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_digit_2.h"

/* ---------- Helpers ---------- */

/* Factorize n (uint64_t) into prime factors.
 * primes[] and exps[] are filled with at most 64 entries.
 * *cnt is set to the number of distinct primes. */
static void factor_uint64(uint64_t n, uint64_t *primes,
                          uint64_t *exps, size_t *cnt)
{
    *cnt = 0;
    uint64_t exp = 0;

    /* factor 2 */
    while ((n & 1ULL) == 0) {
        n >>= 1;
        exp++;
    }
    if (exp > 0) {
        primes[(*cnt)++] = 2;
        exps[(*cnt) - 1] = exp;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            exp = 0;
            while (n % p == 0) {
                n /= p;
                exp++;
            }
            primes[(*cnt)++] = p;
            exps[(*cnt) - 1] = exp;
        }
    }

    /* remaining prime > sqrt(original n) */
    if (n > 1) {
        primes[(*cnt)++] = n;
        exps[(*cnt) - 1] = 1;
    }
}

/* Recursively check that every divisor (except 1) contains digit 2. */
static bool rec_divisors(size_t idx, seq_t cur,
                         uint64_t *primes, uint64_t *exps, size_t cnt)
{
    if (idx == cnt) {
        if (cur != (seq_t)1 && !has_digit_2(cur))
            return false;
        return true;
    }

    seq_t mul = (seq_t)1;
    for (size_t e = 0; e <= exps[idx]; ++e) {
        seq_t next = cur * mul;
        if (!rec_divisors(idx + 1, next, primes, exps, cnt))
            return false;
        mul *= (seq_t)primes[idx];
    }
    return true;
}

/* Test whether a candidate satisfies the sequence definition. */
static bool satisfies_uint64(uint64_t cand)
{
    if (!has_digit_2((seq_t)cand))
        return false;              /* candidate must contain '2' */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_uint64(cand, primes, exps, &cnt);

    /* Composite check: not a single prime factor of multiplicity 1 */
    if (!(cnt == 0 || (cnt == 1 && exps[0] == 1 && primes[0] == cand)))
        /* Pass */
        ;
    else
        return false;              /* candidate is prime */

    /* Each prime factor must contain digit 2 */
    for (size_t i = 0; i < cnt; ++i) {
        if (!has_digit_2((seq_t)primes[i]))
            return false;
    }

    /* All divisors (except 1) must have digit 2 */
    if (!rec_divisors(0, (seq_t)1, primes, exps, cnt))
        return false;

    return true;
}

/* ---------- Sequence cache ---------- */

static seq_t *seq_cache = NULL;   /* dynamic array of sequence values */
static size_t seq_len   = 0;      /* number of cached terms */
static size_t seq_cap   = 0;      /* allocated capacity */
static uint64_t next_candidate = 2; /* next integer to test */

/* Ensure the cache can hold at least 'need' terms. */
static void ensure_cache(size_t need)
{
    if (seq_cap >= need)
        return;

    size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
    while (new_cap < need)
        new_cap *= 2;

    if (seq_cap == 0)
        seq_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    else
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));

    seq_cap = new_cap;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined below offset */
        return (seq_t)0;

    /* Grow cache until it contains at least n terms. */
    while (seq_len < (size_t)n) {
        uint64_t cand = next_candidate++;
        if (satisfies_uint64(cand)) {
            ensure_cache(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)cand;
        }
    }

    return seq_cache[n - 1];
}
