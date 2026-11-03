/*
 * A050711  Inserting a digit '1' between adjacent digits of n makes a prime.
 * https://oeis.org/A050711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 174, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Validated: standard, pass, 70 terms through n = 950 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A050711: the n-th integer whose decimal digits, after inserting
   the digit '1' between every pair of adjacent digits, form a prime.
   We generate terms by incremental search starting from 10 (the
   smallest candidate with two digits).
   The transformation is pure digit manipulation, and primality is
   tested with the provided generic primality routine. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Convert x to the number obtained by inserting a '1' between each pair
   of adjacent decimal digits.  Example: 303 -> 31013. */
static seq_t insert_one(seq_t x)
{
    /* Extract digits of x (least significant first). */
    int digits[32];
    int dlen = 0;
    seq_t tmp = x;

    if (tmp == 0) {
        digits[dlen++] = 0;
    } else {
        while (tmp > 0) {
            digits[dlen++] = (int)(tmp % 10);
            tmp /= 10;
        }
    }

    /* Build the transformed number, most significant digit first,
       inserting '1' between digits while walking backwards. */
    seq_t t = digits[dlen - 1];
    for (int i = dlen - 2; i >= 0; --i) {
        t = t * 10 + 1;
        t = t * 10 + digits[i];
    }
    return t;
}

/* Cache of found terms so repeated calls are efficient. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Next candidate to test; start at the smallest two‑digit number. */
static seq_t cur_candidate = 10;

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n stored. */
    while (cache_len < (size_t)n) {
        seq_t cand = cur_candidate;
        seq_t trans = insert_one(cand);
        if (isPrimeGeneric(trans)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (tmp == NULL) abort();      /* out of memory */
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = cand;
        }
        ++cur_candidate;   /* next candidate */
    }
    return cache[(size_t)n - 1];
}
