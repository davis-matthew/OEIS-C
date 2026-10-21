/*
 * A229877  Palindromic prime numbers == 1 (mod 9).
 * https://oeis.org/A229877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = the nth palindromic prime ≡ 1 (mod 9)
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 10, u64 n <= 1005,
 *            u128 n <= 1005, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

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
