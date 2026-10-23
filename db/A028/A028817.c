/*
 * A028817  Palindromic squares with an odd number of digits.
 * https://oeis.org/A028817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A028817: Palindromic squares with an odd number of digits.
 *
 * The sequence consists of all perfect squares whose decimal representation
 * is a palindrome and whose number of digits is odd.
 * This function computes the n‑th term by generating squares of consecutive
 * integers and testing the palindrome and odd‑digit conditions.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

static seq_t *cache     = NULL; /* cache[0] holds a(1) */
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap   = 0;   /* current allocation capacity */

static idx_t next_root = 1;     /* next integer whose square is to be tested */

/* dynamic array for extracting decimal digits of a number */
static uint8_t *digit_buf   = NULL;
static size_t   digit_cap   = 0;

seq_t a(idx_t n)
{
    if (n == 0) {                     /* undefined for offset 0, but harmless */
        return (seq_t)0;
    }

    /* Ensure the cache contains at least the first n terms. */
    while (cache_size < (size_t)n) {

        /* Square of the current root candidate. */
        seq_t sq = (seq_t)next_root * (seq_t)next_root;

        /* Extract decimal digits of `sq` into `digit_buf`. */
        seq_t temp = sq;
        seq_t ten  = 10;
        seq_t zero = 0;
        size_t len = 0;

        /* Grow digit buffer if needed. */
        size_t required = 64;
        if (digit_cap < required) {
            size_t new_cap = required;
            while (new_cap < required) new_cap *= 2;
            uint8_t *tmp = (uint8_t *)realloc(digit_buf, new_cap);
            if (!tmp) abort();
            digit_buf = tmp;
            digit_cap = new_cap;
        }

        while (temp > zero) {
            seq_t d_t   = temp % ten;
            unsigned d  = (unsigned)d_t;     /* d is guaranteed < 10 */
            if (len == digit_cap) {
                size_t new_cap = digit_cap * 2;
                uint8_t *tmp   = (uint8_t *)realloc(digit_buf, new_cap);
                if (!tmp) abort();
                digit_buf = tmp;
                digit_cap = new_cap;
            }
            digit_buf[len++] = (uint8_t)d;
            temp = temp / ten;
        }

        /* If the number has an even number of digits, skip it. */
        if ((len & 1) == 0) {
            /* even length – not part of the sequence */
        } else {
            /* Check for palindrome: digit_buf contains digits in reverse order. */
            bool pal = true;
            for (size_t i = 0; i < len / 2; ++i) {
                if (digit_buf[i] != digit_buf[len - 1 - i]) {
                    pal = false;
                    break;
                }
            }
            if (pal) {
                /* Append the square to the cache. */
                if (cache_size >= cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = sq;
            }
        }

        ++next_root;
    }

    return cache[n - 1];
}
