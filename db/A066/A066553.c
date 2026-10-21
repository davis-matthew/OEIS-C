/*
 * A066553  Let N =
 *          1357911131517192123252729313335373941434547495153555759616365676971...,
 *          the concatenation of the odd numbers. a(n) is the n-digit number
 *          formed from the digits of N starting from the {n(n-1)/2 +1}th
 *          digit. Omit any leading zeros.
 * https://oeis.org/A066553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A066553: a(n) is the integer formed by the n digits of the
   odd‑number concatenation N = 135791113… starting at position
   n(n−1)/2 + 1 (1‑based).  Leading zeros are omitted automatically
   by the digit accumulation. */
#include <stdint.h>
#include <stdlib.h>

/* Cache of digits from N.  Digits are stored as 0–9 values. */
static unsigned char *n_cache = NULL;   /* array of digits */
static size_t n_cache_len = 0;         /* number of digits currently cached */
static size_t n_cache_cap = 0;         /* allocated capacity of n_cache */
static uint64_t next_odd = 1;          /* next odd number to append */

/* Ensure the cache contains at least 'needed' digits. */
static void ensure_cache(size_t needed)
{
    if (n_cache_len >= needed)
        return;

    /* Grow capacity if necessary. */
    if (n_cache_cap == 0)
        n_cache_cap = needed;
    while (n_cache_cap < needed)
        n_cache_cap <<= 1;          /* double */

    n_cache = (unsigned char *)realloc(n_cache, n_cache_cap);
    if (!n_cache) abort();          /* allocation failed */

    /* Append odd numbers until we have enough digits. */
    while (n_cache_len < needed) {
        uint64_t val = next_odd;
        /* Convert val to decimal digits (least significant first). */
        unsigned char tmp[20];
        size_t pos = 0;
        do {
            tmp[pos++] = (unsigned char)(val % 10);
            val /= 10;
        } while (val > 0);

        /* Append digits in correct order. */
        for (size_t i = 0; i < pos; ++i) {
            if (n_cache_len >= n_cache_cap) {
                /* Should not happen, but guard against overflow. */
                n_cache_cap <<= 1;
                n_cache = (unsigned char *)realloc(n_cache, n_cache_cap);
                if (!n_cache) abort();
            }
            n_cache[n_cache_len++] = tmp[pos - 1 - i];
        }
        next_odd += 2;               /* next odd number */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for 0, but return 0 safely */

    /* 1‑based position of the first digit of a(n). */
    uint64_t start = ((uint64_t)n * (n - 1)) / 2 + 1;
    size_t needed = (size_t)start + n - 1;

    ensure_cache(needed);

    /* Accumulate the n digits starting at start-1 (0‑based index). */
    seq_t val = 0;
    size_t idx = start - 1;
    for (size_t i = 0; i < n; ++i)
        val = val * 10 + n_cache[idx + i];

    return val;
}
