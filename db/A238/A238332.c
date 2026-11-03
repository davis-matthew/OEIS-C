/*
 * A238332  Primes whose digits, after the first, form a nonprime number not
 *          occurring earlier.
 * https://oeis.org/A238332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 2351, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9001 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A238332: primes whose digits after the first form a
   non‑prime number that has not appeared previously.
   The function generates terms on demand and caches them for
   subsequent calls.  It runs in O(n²) time in the worst case, which
   is easily fast enough for n ≤ 10 000. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include <stdint.h>
#include "../utils/num_digits.h"
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

/* Append value to a dynamic array of seq_t. */
static void append_seq_t(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        *arr = (seq_t *)seq_realloc(*arr, *cap, new_cap);
        *cap = new_cap;
    }
    (*arr)[*len] = val;
    (*len)++;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Cached terms of the sequence */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Cached suffixes that have already appeared */
    static seq_t *suffixes = NULL;
    static size_t suffix_len = 0;
    static size_t suffix_cap = 0;

    /* Next integer to test for primality */
    static uint64_t candidate = 2;

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        seq_t cand_seq = (seq_t)candidate;
        if (is_prime_big(cand_seq)) {
            int d = num_digits(cand_seq);
            if (d > 1) {
                seq_t p10 = pow10((idx_t)(d - 1));
                seq_t suffix = cand_seq % p10;
                if (!is_prime_big(suffix)) {
                    bool seen = false;
                    for (size_t i = 0; i < suffix_len; ++i) {
                        if (suffixes[i] == suffix) {
                            seen = true;
                            break;
                        }
                    }
                    if (!seen) {
                        append_seq_t(&cache, &cache_len, &cache_cap, cand_seq);
                        append_seq_t(&suffixes, &suffix_len, &suffix_cap, suffix);
                    }
                }
            }
        }
        candidate++;
    }

    return cache[(size_t)n - 1];
}
