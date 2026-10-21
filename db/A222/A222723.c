/*
 * A222723  Palindromic primes starting with a digit 1.
 * https://oeis.org/A222723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 32, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, pow10, reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A222723: palindromic primes that start with the digit 1. */
/* The function generates palindromes in increasing order and tests each for primality. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/reverse.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* cached palindromic primes          */
static size_t cache_len = 0;         /* number of cached primes            */
static size_t cache_cap = 0;         /* capacity of the cache              */

/* Current palindrome parameters */
static idx_t curr_len = 2;           /* length of the current palindrome  */
static uint64_t curr_x_start = 0;    /* first left‑part value for curr_len */
static uint64_t curr_x_end   = 0;    /* last  left‑part value for curr_len */
static uint64_t curr_x       = 0;    /* current left‑part value            */

/* Ensure the cache can hold at least `min_cap` elements. */
static void
ensure_cache_capacity(size_t min_cap)
{
    if (min_cap <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < min_cap)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                       /* fail fast if allocation fails   */
    cache = tmp;
    cache_cap = new_cap;
}

/* Adjust the left‑part range so that the palindrome starts with digit 1. */
static void
init_range(idx_t len)
{
    idx_t half = (len % 2 == 0) ? (len / 2) : ((len + 1) / 2);
    /* left‑part must begin with 1 and have `half` digits */
    uint64_t base = (uint64_t)pow10(half - 1);     /* 10^(half‑1)  */
    curr_x_start = base;
    curr_x_end   = 2 * base - 1;                  /* 1 followed by `half‑1` nines */
}

/* Generate palindromes until at least `target` primes are cached. */
static void
generate_until(idx_t target)
{
    while (cache_len < (size_t)target)
    {
        if (curr_x == 0)                      /* first time for this length */
        {
            init_range(curr_len);
            curr_x = curr_x_start;
        }

        if (curr_x > curr_x_end)              /* move to next length      */
        {
            curr_len++;
            curr_x = 0;
            continue;
        }

        /* Build the current palindrome. */
        seq_t x_seq = (seq_t)curr_x;
        seq_t pal;
        if (curr_len % 2 == 0)                /* even length              */
        {
            seq_t mult = pow10(curr_len / 2);
            pal = x_seq * mult + reverse(x_seq);
        }
        else                                 /* odd length               */
        {
            seq_t mult = pow10((curr_len - 1) / 2);
            pal = x_seq * mult + reverse((seq_t)(curr_x / 10));
        }

        /* Test for primality and cache if prime. */
        if (is_prime_big(pal))
        {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = pal;
        }

        curr_x++;                             /* next left‑part           */
    }
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                           /* undefined for n = 0, but required to compile. */

    generate_until(n);
    return cache[n - 1];
}
