/*
 * A046495  Primes expressible as the sum of 3 consecutive palindromes.
 * https://oeis.org/A046495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 10, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A046495: primes expressible as the sum of 3 consecutive palindromes.
   For a given n (n ≥ 1) the function returns the n‑th such prime.
   The implementation generates palindromes in increasing order,
   keeps a sliding window of three consecutive palindromes, and
   records their sum whenever it is prime.  The sequence is memoised
   in a static array that grows lazily until it contains at least n
   terms. */

/* core utilities */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* power of ten, exponent ≤ 18 (fits in 64 bits). */
static uint64_t pow10_u64(int exp)
{
    uint64_t r = 1;
    while (exp--) r *= 10;
    return r;
}

/* enlarge the cache if needed – safe for arbitrary-precision seq_t. */
static void ensure_cache(size_t needed, seq_t **cache, size_t *cap)
{
    if (needed > *cap) {
        size_t newcap = (*cap == 0 ? 4 : *cap << 1);
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);          /* fatal if memory allocation fails */
        *cache = tmp;
        *cap   = newcap;
    }
}

/* a(n)  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety – not used in OEIS indexing */

    /* static memoised cache of found primes */
    static seq_t *prime_cache = NULL;
    static size_t cache_len   = 0;
    static size_t cache_cap   = 0;

    /* Palindrome generator state (initialised once). */
    static bool          init          = false;
    static uint64_t      pal_len;      /* length of current palindrome */
    static bool          odd_len;      /* true if pal_len is odd */
    static uint64_t      half_len;     /* digits in the left half (incl. middle) */
    static uint64_t      half_start;   /* first halves value for that length */
    static uint64_t      half_end;     /* last  halves value for that length */
    static uint64_t      cur_half;     /* current left half */
    static seq_t         last2 = 0;    /* two steps back; used in sliding window */
    static seq_t         last1 = 0;    /* one step back */
    static size_t        pal_count = 0;/* number of palindromes generated so far */

    if (!init) {
        pal_len   = 1;
        odd_len   = true;                 /* length 1 is odd */
        half_len  = 1;                    /* (1+1)/2 */
        half_start = pow10_u64(half_len-1);/* 1 */
        half_end   = half_start*10 - 1;   /* 9 */
        cur_half   = half_start;
        init = true;
    }

    while (cache_len < n) {              /* generate until we have enough primes */
        /* Build next palindrome from cur_half. */
        uint64_t p_uint = cur_half;
        uint64_t rev;
        if (odd_len) rev = cur_half / 10;    /* skip middle digit */
        else         rev = cur_half;
        while (rev > 0) {
            p_uint = p_uint * 10 + (rev % 10);
            rev   /= 10;
        }
        seq_t p = (seq_t)p_uint;            /* cast to seq_t for consistency */

        /* After we have at least two earlier palindromes, form the sum. */
        if (pal_count >= 2) {
            seq_t sum = last2 + last1 + p;
            if (is_prime_big(sum)) {
                ensure_cache(cache_len + 1, &prime_cache, &cache_cap);
                prime_cache[cache_len++] = sum;
            }
        }

        /* Slide the window. */
        last2     = last1;
        last1     = p;
        pal_count++;

        /* Advance cur_half / palindrome length. */
        if (cur_half < half_end) {
            cur_half++;
        } else {
            pal_len++;
            odd_len   = (pal_len % 2 == 1);
            half_len  = odd_len ? (pal_len / 2 + 1) : (pal_len / 2);
            half_start = pow10_u64(half_len-1);
            half_end   = half_start * 10 - 1;
            cur_half   = half_start;
        }
    }

    return prime_cache[n-1];             /* indices are 1‑based in OEIS */
}
