/*
 * A138633  Primes of the form 17*k - 9.
 * https://oeis.org/A138633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17*k - 9 where k is the n‑th integer ≥4 such that 17*k-9
 *            is prime
 * Range:     u8 n <= 3, u16 n <= 411, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 6822 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
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

/* static cache for the sequence terms */
static seq_t *cache      = NULL; /* array of cached terms */
static size_t cache_cap  = 0;    /* allocated capacity in terms of seq_t */
static idx_t  cache_len  = 0;    /* number of terms currently stored */
static idx_t  next_k     = 4;    /* next value of k to test */

/* enlarge the cache so it can hold at least 'need' terms */
static void grow_cache(idx_t need)
{
    if ((size_t)need <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < (size_t)need) new_cap <<= 1;

    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort(); /* allocation failed */
    cache     = tmp;
    cache_cap = new_cap;
}

/* ensure that cache contains at least 'n' terms (n ≥ 1) */
static void ensure_up_to(idx_t n)
{
    if (n <= cache_len) return;

    grow_cache(n);
    for (; cache_len < n; ++cache_len)
    {
        for (idx_t k = next_k; ; ++k)
        {
            /* compute value using a wider type to avoid overflow */
            uint64_t temp = 17ULL * k - 9ULL;
            seq_t val = (seq_t)temp;
            if (is_prime_big(val))
            {
                cache[cache_len] = val;
                next_k = k + 1;
                break;
            }
        }
    }
}

/* return the n‑th term (a(1) = 59, a(2) = 127, …) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 1, harmless */
    ensure_up_to(n);
    return cache[n - 1];              /* zero‑based array indexing */
}
