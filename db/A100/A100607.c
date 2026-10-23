/*
 * A100607  Concatenated primes of order 3.
 * https://oeis.org/A100607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   digits_seq, isprime_big, parse_substring, seq_to_dec_string
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate over primes in increasing order and keep those
   that can be represented as concatenations of exactly three primes.
   For each prime, the function checks all splits of its decimal
   representation into three parts, verifies that each part is a prime
   (no leading zeros allowed), and if found, stores the prime in a
   static array.  The function supports out‑of‑order requests for n
   by caching all previously found terms.  It should be fast enough
   for n up to ~10 000. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/isprime_big.h"       /* bool isprime_big(seq_t) */
#include "../utils/seq_to_dec_string.h"/* void seq_to_dec_string(seq_t, char*, size_t) */
#include "../utils/digits_seq.h"       /* idx_t digits_seq(seq_t) */
#include "../utils/parse_substring.h"

static seq_t *cached_vals = NULL;      /* array of found terms */
static idx_t  cached_count = 0;        /* number of stored terms */
static idx_t  cached_capacity = 0;     /* allocated capacity */
static seq_t  next_candidate = 2;      /* next integer to test */

/* Return true if prime p can be split into three primes (no leading zeros). */
static bool is_concat_of_three_primes(seq_t p)
{
    /* Convert p to decimal string. */
    idx_t len = digits_seq(p);
    /* Allocate buffer exactly large enough. */
    char *s = (char *)malloc((size_t)len + 1);
    if (!s) return false;          /* safety */
    seq_to_dec_string(p, s, (size_t)len + 1);

    /* Try all splits i<j where 1≤i<j<len. */
    for (idx_t i = 1; i < len - 1; ++i) {
        if (s[i] == '0') continue;            /* avoid leading zero in second part */
        seq_t a = parse_substring(s, 0, i);
        if (!isprime_big(a)) continue;
        for (idx_t j = i + 1; j < len; ++j) {
            if (s[j] == '0') continue;        /* avoid leading zero in third part */
            seq_t b = parse_substring(s, i, j);
            if (!isprime_big(b)) continue;
            seq_t c = parse_substring(s, j, len);
            if (!isprime_big(c)) continue;
            free(s);
            return true;                      /* found a valid split */
        }
    }
    free(s);
    return false;                              /* no valid split found */
}

/* Append a value to the cached array, resizing if necessary. */
static void cached_append(seq_t val)
{
    if (cached_count == cached_capacity) {
        idx_t new_cap = (cached_capacity == 0) ? 64 : cached_capacity * 2;
        seq_t *new_arr = (seq_t *)realloc(cached_vals, (size_t)new_cap * sizeof(seq_t));
        if (!new_arr) return;                 /* safety: allocation failed */
        cached_vals = new_arr;
        cached_capacity = new_cap;
    }
    cached_vals[cached_count++] = val;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* Build terms until we have at least n terms cached. */
    while (cached_count < n) {
        /* Test if next_candidate is prime. */
        if (isprime_big(next_candidate)) {
            if (is_concat_of_three_primes(next_candidate)) {
                cached_append(next_candidate);
            }
        }
        ++next_candidate;                     /* move to next integer */
    }
    return cached_vals[n - 1];
}
