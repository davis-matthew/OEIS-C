/*
 * A093489  Define the total signature symmetry of a number n to be the number
 *          of values r takes such that n-r and n+r have the same prime
 *          signature. Sequence contains the total signature symmetry of n.
 * https://oeis.org/A093489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 719, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the number of integers r such that 1 ≤ r < n and
   n-r and n+r have the same prime signature
   (the multiset of exponents in their prime factorizations). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "../utils/build_primes_upto.h"

/* Internal structure for a prime‑exponent signature.
   The number of distinct prime factors of numbers ≤ 2·n is at most 16
   for the tested range, so we keep a fixed sized array. */
struct Sig {
    uint8_t len;
    uint8_t exps[16];
};

static struct Sig *sig = NULL;                 /* cached signatures */
static idx_t sig_cap = 0;                      /* number of cached entries */

static uint64_t *primes = NULL;                /* cached primes */
static size_t   prime_count = 0;
static idx_t    prime_limit = 0;

/* Ensure that the array `primes` contains all primes ≤ limit. */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    if (primes)
        free(primes);

    size_t cnt = 0;
    build_primes_upto(limit, &primes, &cnt);
    prime_count = cnt;
    prime_limit = limit;
}

/* Compute the exponent signature of m and store it in out. */
static void compute_sig(uint64_t m, struct Sig *out)
{
    uint8_t exps[16];
    uint8_t len = 0;

    uint64_t rem = m;
    for (size_t i = 0; i < prime_count && primes[i] * primes[i] <= rem; ++i) {
        uint64_t p = primes[i];
        if (rem % p != 0)
            continue;
        uint8_t cnt = 0;
        while (rem % p == 0) {
            rem /= p;
            ++cnt;
        }
        if (cnt > 0) {
            exps[len++] = cnt;
        }
    }
    if (rem > 1)          /* remaining prime factor has exponent 1 */
        exps[len++] = 1;

    /* sort exponents ascending (insertion sort, len ≤ 16) */
    for (uint8_t i = 1; i < len; ++i) {
        uint8_t v = exps[i];
        uint8_t j = i;
        while (j > 0 && exps[j - 1] > v) {
            exps[j] = exps[j - 1];
            --j;
        }
        exps[j] = v;
    }

    out->len = len;
    for (uint8_t i = 0; i < len; ++i)
        out->exps[i] = exps[i];
}

/* Ensure that signatures for every integer 0..limit are computed. */
static void ensure_signatures(idx_t limit)
{
    if (sig_cap > limit)
        return;

    idx_t old_cap = sig_cap;
    idx_t new_cap = limit + 1;          /* indices 0..limit */
    struct Sig *new_sig = (struct Sig *)calloc((size_t)new_cap, sizeof(struct Sig));
    if (!new_sig)                      /* allocation failure – return */
        return;

    if (sig) {
        memcpy(new_sig, sig, (size_t)old_cap * sizeof(struct Sig));
        free(sig);
    }
    sig = new_sig;

    for (idx_t i = old_cap; i <= limit; ++i)
        compute_sig((uint64_t)i, &sig[i]);

    sig_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* The numbers that appear are between 1 and 2*n. */
    idx_t max_num = 2 * n;

    ensure_primes(max_num);
    ensure_signatures(max_num);

    seq_t count = 0;
    for (idx_t r = 1; r < n; ++r) {
        idx_t x = n - r;
        idx_t y = n + r;
        const struct Sig *sx = &sig[x];
        const struct Sig *sy = &sig[y];
        if (sx->len != sy->len)
            continue;
        bool equal = true;
        for (uint8_t k = 0; k < sx->len; ++k) {
            if (sx->exps[k] != sy->exps[k]) {
                equal = false;
                break;
            }
        }
        if (equal)
            count = count + 1;
    }
    return count;
}
