/*
 * A018875  Smallest seventh power that begins with n.
 * https://oeis.org/A018875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest seventh power that begins with n (OEIS A018875)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 84, u128 n <= 10000,
 *            big any n
 * Helpers:   big_pow, digit_len, digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function searches for the least m such that m^7 starts with the
   decimal digits of n, and returns m^7.  Results for previously asked
   values are cached. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, idx_t exp); */
#include "../utils/digit_len.h" /* int digit_len(seq_t x); */
#include "../utils/digits.h"    /* int digits(idx_t x); */
#include "../utils/pow10.h"     /* seq_t pow10(idx_t e); */

static seq_t *cache = NULL;      /* cache[n] = a(n) for n>=1, cache[0] unused */
static idx_t cache_size = 0;     /* number of slots in cache (>= n+1) */

/* Ensure the cache can hold indices up to n */
static void ensure_cache_size(idx_t n)
{
    if (cache_size > n) return;
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    if (!tmp) abort();                /* allocation failure */
    /* initialise new slots to 0 (unset) */
    for (idx_t i = cache_size; i < new_size; ++i) tmp[i] = 0;
    cache = tmp;
    cache_size = new_size;
}

/* main API */
seq_t a(idx_t n)
{
    /* special case for a(0) */
    if (n == 0) return (seq_t)0;

    /* prepare cache */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        *cache = 0;          /* unused slot for index 0 */
        cache_size = 1;
    }
    ensure_cache_size(n);

    if (cache[n] != 0) return cache[n];  /* cache hit */

    /* brute‑force search for the minimal m producing the required prefix */
    seq_t result = 0;
    for (seq_t m = 1;; ++m) {
        seq_t val = big_pow(m, 7);            /* m^7 */
        int len_val = digit_len(val);
        int len_n   = digits(n);
        if (len_val < len_n) continue;        /* too short – can't match */
        seq_t divisor = pow10((idx_t)(len_val - len_n));
        seq_t prefix = val / divisor;         /* leading k digits of val */
        if (prefix == n) {
            result = val;                      /* minimal m found */
            break;
        }
    }

    cache[n] = result;   /* memoise for future calls */
    return result;
}
