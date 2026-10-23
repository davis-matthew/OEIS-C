/*
 * A111185  Let f(k) denote the largest prime factor of k which is not a
 *          palindrome. Sequence gives numbers k such that the sum of the
 *          factorials of the digits of k is equal to f(k) reversed.
 * https://oeis.org/A111185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search with caching – a(n) : largest non‑palindromic prime
 * factor of k reversed equals the sum of factorials of the decimal digits
 * of k. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/rev.h"          /* seq_t rev(seq_t) */

/* factorials of decimal digits 0‑9 as seq_t constants */
static const seq_t fact[10] = {1,1,2,6,24,120,720,5040,40320,362880};

/* true if x is not a palindrome */
static bool is_nonpal(seq_t x) {
    return rev(x) != x;
}

/* largest prime factor of k that is not a palindrome; 0 if none */
static seq_t largest_nonpal_prime(seq_t k) {
    seq_t n   = k;
    seq_t best = 0;

    /* remove factor 2 (palindromic) */
    while ((n % 2) == 0) n /= 2;
    for (seq_t p = 3; p * p <= n; p += 2) {
        if ((n % p) == 0) {
            if (is_nonpal(p) && p > best) best = p;
            while ((n % p) == 0) n /= p;
        }
    }
    if (n > 1 && is_nonpal(n) && n > best) best = n;
    return best;
}

/* true if k satisfies the defining property of the sequence */
static bool good_k(seq_t k) {
    seq_t sum = 0;
    seq_t t   = k;
    while (t > 0) {
        int d = (int)(t % 10);          /* current decimal digit */
        sum += fact[d];
        t   /= 10;
    }
    seq_t f = largest_nonpal_prime(k);
    if (f == 0) return false;
    return sum == rev(f);
}

/* ----- caching mechanism ------------------------------------------------- */

static seq_t *cache          = NULL;   /* stored terms */
static idx_t cache_size      = 0;      /* number of terms stored */
static idx_t cache_cap       = 0;      /* allocated capacity */
static seq_t next_to_check   = 1;      /* next k to evaluate */

/* ensure that cache can hold at least 'required' elements */
static void ensure_cache_cap(idx_t required) {
    if (cache_cap >= required) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < required) new_cap = required;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                  /* out of memory */
    cache     = tmp;
    cache_cap = new_cap;
}

/* return the n‑th term, 1‑based */
seq_t a(idx_t n) {
    while (cache_size < n) {
        if (good_k(next_to_check)) {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = next_to_check;
        }
        next_to_check += 1;
    }
    return cache[n - 1];
}
