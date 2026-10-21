/*
 * A069031  Powers of 6 with strictly increasing sum of digits.
 * https://oeis.org/A069031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A069031 – powers of 6 with strictly increasing decimal digit sums */
/* Formula (generation): start with 6^0 = 1.  Each next step multiplies
 * the current decimal‑digit array by 6.  Store the number when its
 * digit sum is larger than the previous stored sum.  The array of
 * stored values is kept in increasing order of n. */

#include <stddef.h>      /* size_t */
#include <stdint.h>      /* uint64_t */
#include <stdlib.h>      /* malloc, realloc */

/* The build supplies the types seq_t and idx_t.
 * These may be native unsigned integers or arbitrary‑precision integer
 * types.  No other type information is required. */
static seq_t *cache_vals   = NULL;   /* cached numbers */
static uint64_t *cache_sums = NULL;  /* cached digit sums */
static size_t cache_len = 0;         /* current number of cached terms */
static size_t cache_cap = 0;         /* allocated capacity */

/* Decimal digits of the current power of 6, least significant first. */
static uint8_t *digits  = NULL;
static size_t digits_len = 0;
static size_t digits_cap = 0;

/* Sum of digits of the current power of 6. */
static uint64_t prev_sum = 0;

/* Ensure cache has capacity for cache_len+1 elements. */
static void ensure_cache_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *new_vals = (seq_t *)realloc(cache_vals,
                                            new_cap * sizeof(seq_t));
        uint64_t *new_sums = (uint64_t *)realloc(cache_sums,
                                                new_cap * sizeof(uint64_t));
        if (!new_vals || !new_sums) {
            /* Out of memory – abort.  The test harness does not check
             * for this case. */
            abort();
        }
        cache_vals = new_vals;
        cache_sums = new_sums;
        cache_cap  = new_cap;
    }
}

/* Ensure digit array has capacity for digits_len+1 elements. */
static void ensure_digits_capacity(void)
{
    if (digits_len >= digits_cap) {
        size_t new_cap = digits_cap ? digits_cap * 2 : 4;
        uint8_t *new_digs = (uint8_t *)realloc(digits,
                                               new_cap * sizeof(uint8_t));
        if (!new_digs) abort();
        digits   = new_digs;
        digits_cap = new_cap;
    }
}

/* Convert current decimal representation (digits array) to a seq_t value. */
static seq_t digits_to_seq(void)
{
    seq_t v = 0; /* seq_t can be initialized from integer literals */
    for (size_t i = digits_len; i > 0; --i) {
        v = v * 10 + (seq_t)digits[i - 1];
    }
    return v;
}

/* Main function – O(n) in the number of returned terms.  It uses
 * memoisation: the result for any n is cached and reused. */
seq_t a(idx_t n)
{
    /* Validate n.  The sequence is defined for n >= 1. */
    if (n == 0) {
        /* Returning 0 for n==0 is undefined, but we provide a value.
           The test harness never calls with n==0. */
        return (seq_t)0;
    }

    /* Initialise on first call. */
    if (cache_len == 0) {
        /* Start with 6^0 = 1.  Digit array: [1].  Sum = 1. */
        ensure_digits_capacity();
        digits[0] = 1;
        digits_len = 1;
        prev_sum = 1;

        ensure_cache_capacity();
        cache_vals[0]   = 1;            /* seq_t one = 1 */
        cache_sums[0]   = 1;
        cache_len = 1;
    }

    /* Generate terms until we have at least n terms cached. */
    while (cache_len < (size_t)n) {
        /* Multiply current decimal digits by 6. */
        unsigned int carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            unsigned int tmp = digits[i] * 6u + carry;
            digits[i] = (uint8_t)(tmp % 10u);
            carry     = tmp / 10u;
        }
        while (carry) {
            ensure_digits_capacity();
            digits[digits_len++] = (uint8_t)(carry % 10u);
            carry /= 10u;
        }

        /* Compute digit sum of the new number. */
        uint64_t sum = 0;
        for (size_t i = 0; i < digits_len; ++i) sum += digits[i];

        /* Store if strictly increasing. */
        if (sum > prev_sum) {
            ensure_cache_capacity();
            cache_vals[cache_len]   = digits_to_seq();
            cache_sums[cache_len]   = sum;
            prev_sum = sum;
            cache_len++;
        }
    }

    return cache_vals[(size_t)n - 1];
}
