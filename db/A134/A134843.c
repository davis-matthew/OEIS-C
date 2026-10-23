/*
 * A134843  Numbers n not divisible by 10 such that n^2 contains a 0.
 * https://oeis.org/A134843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers n not divisible by 10 such that n^2 contains a decimal *
 *            digit '0'. The function returns the nth such integer (OEIS
 *            offset 1)
 * Range:     u8 none, u16 n <= 97, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Test whether a decimal representation of x contains a digit 0. */
static bool has_zero_digit(seq_t x)
{
    if (x == 0) return true;                      /* 0 has a 0 digit */
    while (x != 0) {
        if ((x % 10) == 0) return true;
        x /= 10;
    }
    return false;
}

/* Persistent cache of the sequence values. */
static seq_t *terms = NULL;          /* a(1), a(2), ... */
static size_t terms_len = 0;         /* number of terms stored */
static size_t terms_cap = 0;         /* allocated capacity of the array */
static idx_t next_candidate = 1;     /* next integer to test (>=1) */

/* Ensure the cache can grow at least to 'need' elements. */
static void grow_cache(size_t need)
{
    if (need <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    if (new_cap < need) new_cap = need;
    seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_arr) return; /* allocation failed – leave cache as is */
    terms = new_arr;
    terms_cap = new_cap;
}

/* Compute and store terms until we have at least 'want' terms. */
static void fill_cache(idx_t want)
{
    if (want <= (idx_t)terms_len) return;      /* already present */

    grow_cache((size_t)want);

    for (;;) {
        /* Skip numbers divisible by 10. */
        if ((next_candidate % 10) == 0) {      /* multiples of 10 are forbidden */
            ++next_candidate;
            continue;
        }

        /* Check whether the square of the candidate contains a 0 digit. */
        seq_t sq = (seq_t)next_candidate * (seq_t)next_candidate;
        if (has_zero_digit(sq)) {
            terms[terms_len++] = (seq_t)next_candidate;
            /* If we have gathered the required number of terms, update
             * next_candidate for the next call and exit. */
            if ((idx_t)terms_len >= want) {
                ++next_candidate;
                break;
            }
        }
        /* Advance to the following integer to test. */
        ++next_candidate;
    }
}

/* Public sequence function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undef. for 0 or negative indices */
    fill_cache(n);
    return terms[n - 1];
}
