/*
 * A091844  a(1) = 4. To get a(n+1), write the string a(1)a(2)...a(n) as xy^k
 *          for words x and y (where y has positive length) and k is
 *          maximized, i.e., k = the maximal number of repeating blocks at the
 *          end of the sequence so far. Then a(n+1) = max(k,4).
 * https://oeis.org/A091844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   recurrence using suffix periodicity
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A091844
   a(1) = 4. For n ≥ 1 let S = a(1)a(2)…a(n) (decimal digits).
   Find the largest k such that a suffix of S is y^k (k copies of a word y, |y|>0).
   Then a(n+1) = max(k, 4). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_to_str.h"

/* global state for memoisation */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;          /* number of sequence elements stored */

static uint8_t *digits   = NULL;       /* concatenated decimal digits of the sequence   */
static size_t  digits_len = 0;
static size_t  digits_cap = 0;

static int *pi_arr        = NULL;      /* KMP prefix function for the reversed string   */
static size_t pi_cap      = 0;

/*--- helpers ----------------------------------------------------------*/
static inline void ensure_digits_capacity(size_t needed)
{
    if (needed > digits_cap) {
        size_t new_cap = digits_cap ? digits_cap * 2 : 64;
        while (new_cap < needed) new_cap *= 2;
        uint8_t *tmp = (uint8_t*)realloc(digits, new_cap);
        digits = tmp;
        digits_cap = new_cap;
    }
}

static inline void ensure_pi_capacity(size_t needed)
{
    if (needed > pi_cap) {
        size_t new_cap = pi_cap ? pi_cap * 2 : 64;
        while (new_cap < needed) new_cap *= 2;
        int *tmp = (int*)realloc(pi_arr, new_cap * sizeof(int));
        pi_arr = tmp;
        pi_cap = new_cap;
    }
}

static void append_value_digits(seq_t val)
{
    /* convert val to decimal string (small buffer is safe for all sizes used) */
    char buf[256];
    size_t len_of_str;
    seq_to_str(val, buf, &len_of_str);
    for (size_t i = 0; i < len_of_str; ++i) {
        uint8_t d = (uint8_t)(buf[i] - '0');
        ensure_digits_capacity(digits_len + 1);
        digits[digits_len++] = d;
    }
}

/* compute the largest exponent of any suffix of the current digit string */
static size_t compute_best_exp(void)
{
    size_t len = digits_len;
    if (len == 0) return 0;          /* should not happen after the first term */
    ensure_pi_capacity(len);
    int *pi = pi_arr;
    pi[0] = 0;
    size_t bestExp = 1;              /* every suffix is at least y^1 */
    int k = 0;
    for (size_t j = 1; j < len; ++j) {
        uint8_t cur = digits[len-1-j];              /* digit at position j in the reverse string */
        while (k > 0 && cur != digits[len-1-k]) k = pi[k-1];
        if (cur == digits[len-1-k]) ++k;
        pi[j] = k;

        size_t L = j + 1;                    /* length of the current suffix */
        size_t p = L - pi[j];                /* minimal period of that suffix */
        size_t exp = (L % p == 0) ? (L / p) : 1;
        if (exp > bestExp) bestExp = exp;
    }
    return bestExp;
}

/*--- sequence function -----------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset is 1 */
    size_t N = (size_t)n;

    while (cache_len < N) {
        seq_t val;
        if (cache_len == 0) {
            val = (seq_t)4;                 /* a(1) = 4 */
        } else {
            size_t bestExp = compute_best_exp();
            val = (seq_t)((bestExp > (size_t)4) ? bestExp : 4);
        }

        /* store value and grow cache */
        cache = (seq_t*)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        cache[cache_len] = val;
        ++cache_len;

        /* append its digits to the global string */
        append_value_digits(val);
    }

    return cache[N-1];
}
