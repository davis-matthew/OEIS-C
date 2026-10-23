/*
 * A174938  a(n) = Sum_{k<=n} A007955(k) * A007955(n-k+1), where A007955(m) =
 *          product of divisors of m.
 * https://oeis.org/A174938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A007955(k) * A007955(n-k+1) * where
 *            A007955(m) = product of the divisors of m
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_spf.h"   /* provides build_spf() */

/* ------------------------------------------------------------------ */
/* Public sequence function declaration (kept external) */

seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/* Cached data ------------------------------------------------------- */

static idx_t *spf       = NULL;     /* smallest prime factor array */
static idx_t  spf_limit = 0;        /* maximum index handled by spf */

/* Cache of divisor‑product values:  b_cache[i] = A007955(i)  */
static seq_t *b_cache   = NULL;
static idx_t  b_cap     = 0;        /* capacity of b_cache */

/* ------------------------------------------------------------------ */
/* Ensure spf[] covers indices up to `limit`. */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit) return;

    if (spf != NULL) free(spf);
    build_spf(limit, &spf);   /* spf[0..limit] is allocated by build_spf */
    spf_limit = limit;
}

/* Ensure b_cache[1..n] is computed; grow the array if needed. */
static void ensure_b(idx_t n)
{
    if (n < 1) return;
    ensure_spf(n);

    /* Resize cache if necessary */
    if (b_cache == NULL) {
        b_cache = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!b_cache) abort();          /* allocation failure */
        b_cap   = n + 1;
    } else if ((size_t)(n + 1) > b_cap) {
        seq_t *tmp = (seq_t*)realloc(b_cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        /* Zero‑initialize the newly added part */
        size_t added = (size_t)(n + 1) - b_cap;
        memset(tmp + b_cap, 0, added * sizeof(seq_t));
        b_cache = tmp;
        b_cap   = (idx_t)(n + 1);
    }

    /* Compute missing entries */
    for (idx_t m = 1; m <= n; ++m) {
        if (b_cache[m] != 0) continue;   /* already computed */

        /* Factorise m using the spf table */
        idx_t t = m;
        int  primes[32];
        int  exps[32];
        int  count = 0;

        while (t > 1) {
            idx_t p = spf[t];
            int    e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            primes[count] = (int)p;
            exps[count]   = e;
            ++count;
        }

        /* Compute τ(m) = product (e_i + 1) */
        idx_t tau = 1;
        for (int i = 0; i < count; ++i) tau *= (idx_t)(exps[i] + 1);

        /* Compute product of divisors: ∏ p_i^{ e_i * τ / 2 } */
        seq_t prod = 1;
        for (int i = 0; i < count; ++i) {
            idx_t exp_total = (idx_t)exps[i] * tau / 2UL;   /* integer division */
            seq_t base      = (seq_t)primes[i];

            /* binary exponentiation of base^exp_total */
            seq_t result   = 1;
            seq_t power    = base;
            idx_t e = exp_total;
            while (e) {
                if (e & 1U) result *= power;
                power *= power;
                e >>= 1U;
            }
            prod *= result;
        }
        /* For m = 1 (no prime factors) prod stays 1 */
        b_cache[m] = prod;
    }
}

/* ------------------------------------------------------------------ */
/* Sequence function implementation */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined only for n ≥ 1 */

    ensure_b(n);

    seq_t sum = 0;
    idx_t half = n / 2;

    /* Sum over k = 1..half, adding each pair twice */
    for (idx_t k = 1; k <= half; ++k) {
        seq_t prod = b_cache[k] * b_cache[n - k + 1];
        sum += prod;
        /* k and n-k+1 are distinct in this range */
        sum += prod;
    }

    /* If n is odd, add the middle term once */
    if (n & 1U) {
        idx_t mid = half + 1;
        sum += b_cache[mid] * b_cache[mid];
    }

    return sum;
}
