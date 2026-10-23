/*
 * A177053  Numbers k such that sum of the decimal digits of k is a quadratic
 *          residue modulo k.
 * https://oeis.org/A177053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   digit_sum, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by sequential search */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/digit_sum.h"
#include "../utils/jacobi_symbol.h"

/* Helper: return true iff k is in the sequence. */
static bool in_seq(seq_t k)
{
    uint64_t mk = (uint64_t)k;          /* candidate fits in 64‑bit for n ≤ 10000 */
    uint64_t s_val = (uint64_t)digit_sum(k);

    /* Special cases for k = 0 (not used) and k = 1 */
    if (mk == 0) return false;
    if (mk == 1) return true;

    /* Handle power‑of‑2 factor e₂(k) */
    int e2 = 0;
    uint64_t tmp = mk;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        ++e2;
    }
    if (e2 > 0) {
        if (e2 == 1) {
            /* modulo 2: all residues are quadratic residues */
        } else if (e2 == 2) {
            uint64_t r = s_val % 4ULL;
            if (r != 0ULL && r != 1ULL) return false;   /* QR mod 4 are {0,1} */
        } else { /* e2 >= 3 */
            /* Compute v₂(s) */
            int v = 0;
            uint64_t t = s_val;
            while ((t & 1ULL) == 0) {
                t >>= 1;
                ++v;
            }
            if (v >= e2) {
                /* s ≡ 0 mod 2^e, always a residue */
            } else if ((v & 1) != 0) {          /* v odd */
                return false;
            } else if ((t % 8ULL) != 1ULL) {   /* odd part ≢ 1 (mod 8) */
                return false;
            }
        }
    }

    /* Remaining odd part of k */
    uint64_t rem = mk >> e2;
    if (rem == 1) return true; /* k is a power of 2 only */

    /* Trial division up to sqrt(rem) */
    for (uint64_t p = 3; p * p <= rem; p += 2) {
        if (rem % p != 0) continue;

        /* exponent of p in k */
        int exp_k = 0;
        while (rem % p == 0) {
            rem /= p;
            ++exp_k;
        }

        /* exponent of p in s (digit sum) */
        int exp_s = 0;
        uint64_t u = s_val;
        while (u % p == 0) {
            u /= p;
            ++exp_s;
        }

        /* Decision */
        if (exp_s >= exp_k) {
            /* s divisible by p^exp_k → always a residue */
        } else if ((exp_s & 1) != 0) {
            return false;  /* odd exponent < exp_k => not a residue */
        } else {
            /* check odd part modulo p */
            if (jacobi_symbol(u % p, p) != 1) return false;
        }
    }

    /* If rem > 1, it is a remaining prime factor. */
    if (rem > 1) {
        uint64_t p = rem;          /* exponent in k is 1 */
        int exp_k = 1;

        int exp_s = 0;
        uint64_t u = s_val;
        while (u % p == 0) {
            u /= p;
            ++exp_s;
        }

        if (exp_s >= exp_k) {
            /* ok */
        } else if ((exp_s & 1) != 0) {
            return false;
        } else {
            if (jacobi_symbol(u % p, p) != 1) return false;
        }
    }

    return true;
}

/* Memoized storage for the sequence */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; unused for n==0 */

    /* Lazy allocation of cache */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);       /* minimal error handling */
    }

    /* Grow cache to contain at least n terms. */
    if (cache_len < (size_t)n) {
        seq_t candidate = (cache_len == 0) ? (seq_t)1 : cache[cache_len - 1] + (seq_t)1;

        while (cache_len < (size_t)n) {
            if (in_seq(candidate)) {
                /* Ensure capacity */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!new_cache) exit(1);
                    cache = new_cache;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = candidate;
            }
            ++candidate;
        }
    }

    return cache[n - 1];
}
