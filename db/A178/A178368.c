/*
 * A178368  Numbers with rounded up arithmetic mean of digits = 8.
 * https://oeis.org/A178368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th positive integer x with
 *            ceil(digit_sum(x)/digit_count(x)) = 8 (rounded‑up arithmetic
 *            mean of digits equals 8)
 * Range:     u8 n <= 8, u16 n <= 604, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build; do not define them */
typedef __typeof__(*(seq_t *)0) seq_t;
typedef __typeof__(*(idx_t *)0) idx_t;

/* Memoization of generated terms */
static seq_t *cache = NULL;     /* dynamically allocated array of seq_t */
static size_t cached = 0;       /* number of terms already stored */
static size_t capacity = 0;     /* allocated capacity of cache */
static seq_t next_candidate = 1;/* next integer to test */

/* Ensure cache has room for at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (capacity >= need)
        return;
    size_t new_cap = (capacity < 16) ? 16 : capacity * 2;
    while (new_cap < need)
        new_cap *= 2;
    /* cast void* to seq_t* explicitly so the code compiles in C++ */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* memory exhausted */
    cache = tmp;
    capacity = new_cap;
}

/* Push a new term onto the cache */
static void push_term(seq_t val)
{
    ensure_capacity(cached + 1);
    cache[cached++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for index 0, return 0 as placeholder */

    /* If we have already produced enough terms, return directly */
    if (cached >= (size_t)n)
        return cache[(size_t)n - 1];

    /* Generate terms until we reach the required index */
    while (cached < (size_t)n) {
        seq_t cur = next_candidate;
        /* Compute digit sum and digit count */
        int sum = 0;
        int len = 0;
        seq_t v = cur;
        while (v > 0) {
            int digit = (int)(v % 10);
            sum += digit;
            v /= 10;
            len++;
        }
        /* Check the rounded‑up mean condition */
        if (sum > 7 * len && sum <= 8 * len) {
            push_term(cur);
        }
        next_candidate++;  /* move to the next integer for future searches */
    }

    return cache[(size_t)n - 1];
}
