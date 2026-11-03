/*
 * A229877  Palindromic prime numbers == 1 (mod 9).
 * https://oeis.org/A229877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = the nth palindromic prime ≡ 1 (mod 9)
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 1005, u64 n <= 1005,
 *            u128 n <= 1005, big any n
 * Validated: standard, pass, 49 terms through n = 943 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* state for generating odd‑length palindromes */
static uint64_t pow10k   = 100;   /* 10^curr_k, curr_k starts at 2 */
static uint64_t left_val = 10;    /* 10^(curr_k-1) */
static uint64_t mult     = 10;    /* 10^(curr_k-1) */
static uint64_t end_val  = 99;    /* 10^curr_k - 1 */

/* extend cache to contain at least n terms */
static void ensure_cache(idx_t n)
{
    while ((idx_t)cache_size < n)
    {
        /* construct odd‑length palindrome from left_val */
        uint64_t temp = left_val / 10;   /* skip the middle digit when mirroring */
        uint64_t rev  = 0;
        while (temp)
        {
            rev = rev * 10 + temp % 10;
            temp /= 10;
        }
        uint64_t pal = left_val * mult + rev;   /* fits in 64 bits for all required n */

        if (pal % 9ULL == 1ULL && is_prime_big((seq_t)pal))
        {
            /* grow cache if necessary */
            if (cache_size == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)pal;
        }

        /* advance to next left half */
        left_val++;
        if (left_val > end_val)
        {
            /* move to the next digit length for the left part */
            pow10k *= 10;                /* 10^(curr_k) becomes 10^(curr_k+1) */
            left_val = pow10k / 10;      /* 10^curr_k */
            mult = left_val;             /* 10^curr_k */
            end_val = pow10k - 1;        /* 10^(curr_k+1) - 1 */
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;       /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n - 1];
}
