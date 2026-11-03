/*
 * A090227  Beginning with 11, the smallest prime not already used beginning
 *          with the number obtained by placing the most significant digit to
 *          the right of the least significant digit.
 * https://oeis.org/A090227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth term of OEIS A090227 – the smallest unused prime
 *            that starts with the number obtained by rotating the
 *            most‑significant digit of the previous term to the
 *            least‑significant position
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

static seq_t *cache = NULL;      /* array of already generated terms     */
static size_t cache_size = 0;    /* number of terms stored               */
static size_t cache_cap  = 0;    /* allocated capacity                   */

/* ------------------------------------------------------------------ */
/* Ensure at least <needed> capacity for the cache                 */
/* ------------------------------------------------------------------ */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Linear search for a value in the cache (used to prevent repeats)  */
/* ------------------------------------------------------------------ */
static bool cache_contains(seq_t v)
{
    for (size_t i = 0; i < cache_size; ++i)
        if (cache[i] == v) return true;
    return false;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* below offset – undefined */

    /* Initialise the cache with the first term if empty. */
    if (cache_size == 0) {
        ensure_cache_capacity(1);
        cache[0] = 11;
        cache_size = 1;
    }

    /* Generate terms up to the requested index. */
    while (cache_size < n) {
        seq_t last = cache[cache_size - 1];

        /* Compute the rotated prefix as an integer:
           prefix = (last without most‑significant digit) * 10
                     + most‑significant digit. */
        idx_t   d   = num_digits(last);
        seq_t   pow_d_1 = pow10(d - 1);             /* 10^(d-1)            */
        seq_t   msd = last / pow_d_1;               /* most‑significant digit */
        seq_t   rest = last % pow_d_1;              /* the remaining part   */
        seq_t   prefix = rest * 10 + msd;           /* rotated number       */

        /* Search for the smallest unused prime starting with the prefix. */
        seq_t  next = 0;
        bool   found = false;
        for (size_t suffix_len = 0; !found; ++suffix_len) {
            seq_t pow_suf = pow10((idx_t)suffix_len);   /* 10^suffix_len    */
            seq_t base   = prefix * pow_suf;            /* prefix…0         */
            seq_t limit  = (prefix + 1) * pow_suf;      /* next block      */

            for (seq_t cand = base; cand < limit; ++cand) {
                if (!cache_contains(cand) && isPrimeGeneric(cand)) {
                    next = cand;
                    found = true;
                    break;
                }
            }
        }

        /* Store the newly found term. */
        ensure_cache_capacity(cache_size + 1);
        cache[cache_size++] = next;
    }

    return cache[n - 1];
}
