/*
 * A075800  Palindromic numbers which are products of an odd number of
 *          distinct primes.
 * https://oeis.org/A075800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive palindrome that is square‑free and
 *            whose number of distinct prime factors is odd. Calculation is
 *            done by generating palindromes in increasing order and testing
 *            the conditions
 * Range:     u8 n <= 12, u16 n <= 228, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse_partial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/reverse_partial.h"

/* --------------------------------------------------------------------- */
/*  Utility helpers                                                      */
/* --------------------------------------------------------------------- */

/* Powers of ten – enough for 64‑bit numbers (they also work for
   big numbers when used only for indexing) */
static const uint64_t pow10[20] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL, 1000000ULL,
    10000000ULL, 100000000ULL, 1000000000ULL, 10000000000ULL,
    100000000000ULL, 1000000000000ULL, 10000000000000ULL,
    100000000000000ULL, 1000000000000000ULL, 10000000000000000ULL,
    100000000000000000ULL, 1000000000000000000ULL, 10000000000000000000ULL
};

/* Construct a palindrome from the integer 'half'.
   If 'len' is odd, the middle digit of 'half' is not duplicated. */
static uint64_t make_palindrome(uint64_t half, int len)
{
    int half_len = (len + 1) / 2;
    if ((len & 1) == 0)                     /* even length */
    {
        uint64_t rev = reverse_partial(half, half_len);
        return half * pow10[half_len] + rev;
    }
    else                                    /* odd length */
    {
        uint64_t half_no_last = half / 10ULL;
        uint64_t rev = reverse_partial(half_no_last, half_len - 1);
        return half * pow10[half_len - 1] + rev;
    }
}

/* --------------------------------------------------------------------- */
/*  Prime generation (once, up to a fixed limit)                        */
/* --------------------------------------------------------------------- */
#define PRIME_LIMIT 2000000U                 /* sufficient for sqrt(10^12) */

static uint32_t *prime_list = NULL;
static size_t     prime_count = 0;

static void init_primes(void)
{
    if (prime_list != NULL) return;  /* already done */

    uint8_t *comp = (uint8_t *)calloc(PRIME_LIMIT + 1, 1);
    prime_list = (uint32_t *)malloc(PRIME_LIMIT * sizeof(uint32_t));
    size_t sz = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
    {
        if (!comp[i])
        {
            if (sz == PRIME_LIMIT) break;
            prime_list[sz++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT)
            {
                for (uint64_t j = (uint64_t)i * i; j <= PRIME_LIMIT; j += i)
                    comp[j] = 1;
            }
        }
    }
    free(comp);
    prime_count = sz;
}

/* --------------------------------------------------------------------- */
/*  Test whether a value is square‑free and has an odd number of
    distinct prime factors.                                              */
/* --------------------------------------------------------------------- */
static bool qualifies(uint64_t val)
{
    size_t distinct = 0;
    uint64_t n = val;
    for (size_t i = 0; i < prime_count &&
         (uint64_t)prime_list[i] * prime_list[i] <= n; ++i)
    {
        uint32_t p = prime_list[i];
        if (n % p == 0)
        {
            ++distinct;
            n /= p;
            if (n % p == 0) return false;   /* repeated factor */
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) ++distinct;                 /* remaining prime > sqrt(val) */
    return (distinct & 1) != 0;            /* true if odd */
}

/* --------------------------------------------------------------------- */
/*  Cache for sequence values                                            */
/* --------------------------------------------------------------------- */
static seq_t *cache_vals = NULL;
static idx_t  cache_len  = 0;
static idx_t  cache_cap  = 0;

static void cache_add(seq_t v)
{
    if (cache_len == cache_cap)
    {
        cache_cap = cache_cap ? cache_cap * 2 : 64;
        cache_vals = (seq_t *)realloc(cache_vals, cache_cap * sizeof(seq_t));
    }
    cache_vals[cache_len++] = v;
}

/* --------------------------------------------------------------------- */
/*  Palindrome generator state                                          */
/* --------------------------------------------------------------------- */
static int  gen_len          = 1;                     /* current length   */
static uint64_t gen_start_half = 1;                    /* first half for cur len */
static uint64_t gen_end_half   = 10;                   /* exclusive end */
static uint64_t gen_cur_half   = 1;                   /* current half */

/* Return the next palindrome and advance the generator state. */
static uint64_t next_palindrome(void)
{
    uint64_t pal = make_palindrome(gen_cur_half, gen_len);

    ++gen_cur_half;
    if (gen_cur_half == gen_end_half)
    {
        ++gen_len;
        int half_len = (gen_len + 1) / 2;
        gen_start_half = pow10[half_len - 1];
        gen_end_half   = pow10[half_len];
        gen_cur_half   = gen_start_half;
    }
    return pal;
}

/* --------------------------------------------------------------------- */
/*  Public entry point                                                   */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety; offset is 1 */

    init_primes();

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        uint64_t candidate = next_palindrome();
        if (qualifies(candidate))
            cache_add((seq_t)candidate);   /* cast to seq_t */
    }
    return cache_vals[n - 1];
}
