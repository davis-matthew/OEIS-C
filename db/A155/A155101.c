/*
 * A155101  Nonprimes k such that the sum of prime factors of composite(k) is
 *          an even composite.
 * https://oeis.org/A155101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   nonprimes k such that the sum of prime factors of composite(k)
 *            is an even composite. a(1) = 1, a(2) = 4, a(3) = 8, a(4) = 9,
 *            a(5) = 10, a(6) = 12,
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --- Basic helpers ------------------------------------------------------- */

/* Return true if n is a prime (for 64‑bit unsigned values). */
static bool is_prime_small(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Return sum of prime factors of n (with multiplicity). */
static uint64_t sum_prime_factors(uint64_t n)
{
    uint64_t sum = 0;
    for (uint64_t d = 2; d * d <= n; ++d)
    {
        while (n % d == 0)
        {
            sum += d;
            n /= d;
        }
    }
    if (n > 1) sum += n;
    return sum;
}

/* ------------------------------------------------------------------------ */

/* Return the k‑th composite number: composite(1)=4, composite(2)=6, ... */
static uint64_t get_composite(uint64_t k)
{
    static uint64_t comp_idx = 0;   /* number of composites found so far   */
    static uint64_t comp_num = 3;   /* last number tested (before first 4) */

    /* Generate composites until we have at least k of them. */
    while (comp_idx < k)
    {
        ++comp_num;
        if (comp_num >= 4 && !is_prime_small(comp_num))
            ++comp_idx;
    }
    return comp_num;  /* now comp_num is the k‑th composite */
}

/* ------------------------------------------------------------------------ */

/* Cache for the sequence values so that repeated calls are cheap. */
static seq_t *terms      = NULL;   /* array of stored sequence values   */
static size_t terms_cap  = 0;      /* current allocation size          */
static size_t terms_len  = 0;      /* number of terms stored            */
static uint64_t last_k   = 0;      /* highest 'k' processed so far       */

/* Ensure the cache array has space for at least needed elements. */
static void ensure_cache(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) { terms = NULL; terms_cap = 0; } else { terms = tmp; terms_cap = new_cap; }
}

/* ------------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined below offset; harmless */

    /* Return cached value if already computed. */
    if (terms_len >= (size_t)n)
        return terms[(size_t)n - 1];

    /* Extend cache until we have at least n terms. */
    for (uint64_t k = last_k + 1; ; ++k)
    {
        last_k = k;

        /* Consider only non‑prime k (including k==1). */
        if (!is_prime_small(k))
        {
            uint64_t c   = get_composite(k);            /* c = composite(k)           */
            uint64_t sum = sum_prime_factors(c);        /* sum of prime factors        */

            /* sum must be even composite (i.e., even and not prime). */
            if ((sum & 1ULL) == 0 && !is_prime_small(sum))
            {
                /* Store the term */
                ensure_cache(terms_len + 1);
                terms[terms_len++] = (seq_t)k;

                if (terms_len >= (size_t)n)
                    return terms[(size_t)n - 1];
            }
        }
    }
}
