/*
 * A222723  Palindromic primes starting with a digit 1.
 * https://oeis.org/A222723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 54 terms through n = 997 (2026-11-03)
 * Helpers:   is_prime_u64, mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A222723: palindromic primes that start with the digit 1. */
/* The function generates palindromes in increasing order and tests each for primality. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/reverse_digits.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
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
            pal = x_seq * mult + reverse_digits(x_seq);
        }
        else                                 /* odd length               */
        {
            seq_t mult = pow10((curr_len - 1) / 2);
            pal = x_seq * mult + reverse_digits((seq_t)(curr_x / 10));
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
