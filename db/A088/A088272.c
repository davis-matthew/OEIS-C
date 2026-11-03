/*
 * A088272  Palindromic primes that yield a prime when sandwiched between two
 *          9's.
 * https://oeis.org/A088272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the nth palindromic prime p such that the decimal number
 *            formed by sandwiching p between two 9's (i.e. 9p9) is also
 *            prime. The function searches integers in ascending order, tests
 *            for palindrome, primality of p, and primality of 9p9
 * Range:     u8 none, u16 n <= 1, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-03)
 * Helpers:   is_palindrome_base, is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_palindrome_base.h"
#include "../utils/num_digits.h"
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* The next integer to test */
    static idx_t next_candidate = 2;

    /* If the cache already covers the requested index, return it */
    if (cache_len >= n)
        return cache[n - 1];

    /* Generate terms until we have at least n */
    for (idx_t x = next_candidate; ; ++x)
    {
        seq_t p = (seq_t)x;

        /* Skip non‑palindromes quickly */
        if (!is_palindrome_base(p, 10))
            continue;

        /* Must be a prime itself */
        if (!is_prime_big(p))
            continue;

        /* Build 9p9 */
        int d = num_digits(p);            /* number of decimal digits of p */
        seq_t t = pow10((idx_t)(d + 1));  /* 10^(d+1) */
        seq_t sandwich = ((seq_t)9) * t + p * ((seq_t)10) + ((seq_t)9);

        /* 9p9 must also be prime */
        if (!is_prime_big(sandwich))
            continue;

        /* Store the qualifying prime */
        append_candidate(&cache, &cache_len, &cache_cap, p);

        /* If we reached the desired term, stop */
        if (cache_len >= n) {
            next_candidate = x + 1;
            break;
        }
    }
    return cache[n - 1];
}
