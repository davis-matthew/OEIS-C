/*
 * A168322  a(1)=3,a(2)=5; a(n+1)=smallest prime number > a(n-1) such that the
 *          sum of any three consecutive terms is a prime.
 * https://oeis.org/A168322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=3, a(2)=5; for n≥3, * a(n)=the smallest prime greater than
 *            a(n‑2) * that makes a(n‑2)+a(n‑1)+a(n) also prime
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    /* static cache of computed terms, 1‑based indices stored in 0‑based array */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;   /* current allocated capacity */
    static idx_t terms_len = 0;    /* number of terms already computed (≤ terms_cap) */

    if (n == 0) return 0; /* undefined, but guard against illegal argument */

    /* grow cache if necessary */
    if (terms_len < n)
    {
        /* ensure enough space for n terms */
        ensure_cache(n, &terms, &terms_cap);

        /* compute missing terms */
        for (idx_t i = terms_len + 1; i <= n; ++i)
        {
            if (i == 1)
                terms[i - 1] = (seq_t)3;        /* a(1) */
            else if (i == 2)
                terms[i - 1] = (seq_t)5;        /* a(2) */
            else
            {
                seq_t prev2 = terms[i - 3];     /* a(i‑2) */
                seq_t prev1 = terms[i - 2];     /* a(i‑1) */
                seq_t candidate = prev2 + (seq_t)1; /* start just above prev2 */

                for (;; ++candidate)
                {
                    if (!is_prime_big(candidate))
                        continue;

                    seq_t sum = prev2 + prev1 + candidate;
                    if (is_prime_big(sum))
                    {
                        terms[i - 1] = candidate; /* a(i) */
                        break;
                    }
                }
            }
        }

        terms_len = n; /* update length after filling up to n */
    }

    return terms[n - 1];
}
