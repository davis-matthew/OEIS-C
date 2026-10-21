/*
 * A071906  Sum of digits of 2^n (mod 2).
 * https://oeis.org/A071906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (sum of decimal digits of 2^n) mod 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 0, a(2) = 0, ... (OEIS A071906) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Memoisation table for parity values */
static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static idx_t max_cached = (idx_t)-1;   /* largest n with a stored value */

/* Digital representation of the current power of two (least‑significant digit first) */
static unsigned char *dec_num = NULL;
static size_t num_len = 0;   /* number of used digits */
static size_t num_cap = 0;   /* allocated capacity */

/* Ensure cache can hold values up to at least n */
static void ensure_cache_capacity(idx_t n)
{
    if (n < cache_capacity) return;
    size_t new_cap = (cache_capacity == 0) ? 16 : cache_capacity;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_capacity = new_cap;
}

/* Ensure the digit array can hold at least len digits */
static void ensure_num_capacity(size_t len)
{
    if (len <= num_cap) return;
    size_t new_cap = (num_cap == 0) ? 16 : num_cap;
    while (new_cap <= len) new_cap <<= 1;
    dec_num = (unsigned char *)realloc(dec_num, new_cap);
    num_cap = new_cap;
}

/* Compute parity (mod 2) of the sum of digits of the current decimal number */
static seq_t parity_of_digits(void)
{
    seq_t p = 0;
    for (size_t i = 0; i < num_len; ++i)
        p ^= (seq_t)(dec_num[i] & 1);
    return p;
}

/* Multiply the current decimal number by 2 */
static void multiply_by_two(void)
{
    unsigned int carry = 0;
    for (size_t i = 0; i < num_len; ++i) {
        unsigned int prod = dec_num[i] * 2 + carry;
        dec_num[i] = prod % 10;
        carry = prod / 10;
    }
    while (carry) {
        ensure_num_capacity(num_len + 1);
        dec_num[num_len++] = carry % 10;
        carry /= 10;
    }
}

/* External function required by the specification */
seq_t a(idx_t n)
{
    /* Initialise on first call */
    if (!cache) {
        ensure_cache_capacity(0);
        ensure_num_capacity(1);
        dec_num[0] = 1;        /* 2^0 = 1 */
        num_len = 1;
        cache[0] = 1;         /* parity of digit sum of 1 is 1 */
        max_cached = 0;
    }

    if (n <= max_cached) return cache[n];

    for (idx_t k = max_cached + 1; k <= n; ++k) {
        multiply_by_two();
        seq_t p = parity_of_digits();
        ensure_cache_capacity(k);
        cache[k] = p;
        max_cached = k;
    }
    return cache[n];
}
