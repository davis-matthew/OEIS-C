/*
 * A045725  Fibonacci numbers having initial digit '1'.
 * https://oeis.org/A045725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Fibonacci numbers whose first decimal digit is 1, in increasing
 *            order
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-03)
 * Helpers:   mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static seq_t f_prev = 0;
    static seq_t f_curr = 0;
    static int initialized = 0;
    static seq_t last_seen = 0;

    if (!initialized) {
        f_prev = (seq_t)1;
        f_curr = (seq_t)1;
        initialized = 1;
    }

    /* Ensure the first qualifying Fibonacci number (1) is in the cache. */
    if (cache_size == 0) {
        grow_seq(&cache, &cache_cap, 1);
        cache[0] = (seq_t)1;
        cache_size = 1;
        last_seen = (seq_t)1;
    }

    /* Compute qualifying Fibonacci numbers until we have n of them. */
    while (cache_size < n) {
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;

        int digits = num_digits(f_next);
        seq_t pow10_val = pow10((idx_t)(digits - 1));
        seq_t leading = f_next / pow10_val;

        if (leading == (seq_t)1 && f_next != last_seen) {
            grow_seq(&cache, &cache_cap, cache_size + 1);
            cache[cache_size++] = f_next;
            last_seen = f_next;
        }
    }

    return cache[n - 1];
}
