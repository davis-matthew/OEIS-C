/*
 * A128375  Palindromic primes that start and end with 9.
 * https://oeis.org/A128375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 50 terms through n = 979 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th palindromic prime that starts and ends
 * with the digit 9.  It is generated incrementally by constructing
 * odd‑length palindromes whose first and last digit are 9, testing each
 * for primality with is_prime_big, and storing the primes found in a
 * static cache.  Subsequent calls reuse previously computed terms. */
/* Note: Indexing follows OEIS: a(1) is the first term. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

seq_t a(idx_t n)
{
    /* Cache of primes that have been found so far. */
    static seq_t *prime_cache   = NULL;
    static size_t cache_len     = 0;
    static size_t cache_cap     = 0;

    /* State of the incremental palindrome generator. */
    static bool   initialized   = false;
    static int    cur_len       = 3;      /* current odd length (>=3)   */
    static int    k             = 1;      /* k = (cur_len-1)/2           */
    static seq_t  powk          =  0;      /* 10^k                        */
    static seq_t  left_start    =  0;      /* 9 * 10^k                    */
    static idx_t  d             =  0;      /* current offset within left   */
    static idx_t  max_d         =  0;      /* 10^k (type idx_t)           */

    /* Initialise generator on first call. */
    if (!initialized) {
        powk      = pow10((idx_t)k);
        left_start= 9 * powk;
        max_d     = (idx_t)powk;
        initialized = true;
    }

    /* Generate further primes until we have at least n of them. */
    while (cache_len < n) {
        seq_t pal;

        /* Find the next palindrome prime. */
        while (true) {
            /* Advance to the next length if we have exhausted all left
             * prefixes of the current length. */
            if (d >= max_d) {
                cur_len += 2;
                k = (cur_len - 1) / 2;
                powk      = pow10((idx_t)k);
                left_start= 9 * powk;
                max_d     = (idx_t)powk;
                d         = 0;
            }

            /* Construct the palindrome from the current left part. */
            seq_t left = left_start + (seq_t)d;
            d++;

            pal = left;
            seq_t t = left / 10;
            while (t > 0) {
                pal = pal * 10 + (t % 10);
                t   = t / 10;
            }

            /* Accept it if it is prime. */
            if (is_prime_big(pal)) {
                append_candidate(&prime_cache, &cache_len, &cache_cap, pal);
                break;
            }
        }
    }

    return prime_cache[n - 1];
}
