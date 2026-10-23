/*
 * A015992  Eleven iterations of Reverse and Add are needed to reach a
 *          palindrome.
 * https://oeis.org/A015992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by brute‑force search: numbers that require exactly
   11 iterations of Reverse‑and‑Add to reach a palindrome. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: reverse the decimal representation of x */
static seq_t reverse_decimal(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + (x % 10);
        x   = x / 10;
    }
    return rev;
}

/* Helper: test whether x is a palindrome in base 10 */
static bool is_palindrome(seq_t x)
{
    return x == reverse_decimal(x);
}

/* Static state for incremental generation */
static seq_t *cached = NULL;
static size_t cached_size = 0;
static size_t cached_cap  = 0;
static seq_t next_start = 0;

/* Grow the cache array when needed */
static void grow_cache(void)
{
    size_t new_cap = cached_cap ? cached_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(cached, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cached = tmp;
    cached_cap = new_cap;
}

/* Generate terms up to (and including) index n */
static void generate_up_to(idx_t n)
{
    while (cached_size <= (size_t)n) {
        seq_t x   = next_start;
        seq_t tmp = x;
        bool ok   = false;

        for (int it = 1; it <= 11; ++it) {
            seq_t rev = reverse_decimal(tmp);
            tmp = tmp + rev;          /* one Reverse‑and‑Add iteration */
            if (is_palindrome(tmp)) {
                if (it == 11) ok = true;
                break;
            }
        }

        if (ok) {
            if (cached_size == cached_cap) grow_cache();
            cached[cached_size++] = x;
        }
        ++next_start;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Initialise static state on first call */
    if (cached == NULL) {
        cached = NULL;
        cached_cap = 0;
        cached_size = 0;
        next_start = 0;
    }
    generate_up_to(n);
    return cached[(size_t)n];
}
