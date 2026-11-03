/*
 * A076053  Rearrangement of natural numbers such that the 2n-th partial sum
 *          is composite and (2n-1)-th partial sum is prime.
 * https://oeis.org/A076053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greedy construction of OEIS A076053:
   permutation of natural numbers such that the (2n–1)-st partial sum is prime
   and the 2n‑th partial sum is composite.  */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
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

/* Static tables for caching the sequence and used numbers. */
static seq_t *cache     = NULL;      /* cache[0] = a(1) */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* allocated capacity of cache */

static bool *used   = NULL;          /* used[i] = true if integer i is used */
static size_t used_len = 0;          /* allocated length of used array */

static seq_t cumulative_sum = 0;     /* partial sum of the sequence */

/* Ensure that `used` has room for index `k` (1‑based).  */
static void ensure_used_capacity(idx_t k)
{
    if (used_len > (size_t)k) return;
    size_t new_len = used_len ? used_len * 2 : 8;
    while (new_len <= (size_t)k) new_len <<= 1;

    bool *tmp = (bool *)realloc(used, new_len * sizeof(bool));
    if (!tmp) abort();
    if (used_len < new_len)
        memset(tmp + used_len, 0, (new_len - used_len) * sizeof(bool));
    used = tmp;
    used_len = new_len;
}

/* Ensure that `cache` can hold at least `k` terms.  */
static void ensure_cache_capacity(idx_t k)
{
    if (cache_cap >= (size_t)k) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < (size_t)k) new_cap <<= 1;

    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term of A076053 (n ≥ 1).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0 */

    /* Build the sequence up to n.  */
    ensure_cache_capacity(n);
    while (cache_len < (size_t)n)
    {
        /* We are about to compute the (cache_len+1)-th term. */
        bool want_prime = ((cache_len + 1) & 1UL) != 0;  /* odd index → prime */

        for (idx_t x = 1; ; ++x)
        {
            ensure_used_capacity(x);
            if (used[(size_t)x]) continue;  /* already used */

            /* Test the new partial sum.  */
            seq_t new_sum = cumulative_sum + (seq_t)x;
            bool ok;
            if (want_prime)
                ok = new_sum > (seq_t)1 && is_prime_big(new_sum);
            else
                ok = new_sum > (seq_t)1 && !is_prime_big(new_sum);

            if (ok)
            {
                cache[cache_len++] = (seq_t)x;
                used[(size_t)x] = true;
                cumulative_sum = new_sum;
                break;
            }
        }
    }

    return cache[(size_t)n - 1];
}
