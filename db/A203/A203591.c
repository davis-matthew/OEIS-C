/*
 * A203591  Numbers such that each digit is the sum of two or more other
 *          digits.
 * https://oeis.org/A203591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 23, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Brute‑force search for numbers where each decimal digit
   equals the sum of two or more other digits in the same number. The
   function returns the nth such number (a(1)=10001). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Least‑significant set bit index (x > 0). */
static int lsb_index(unsigned int x)
{
    int i = 0;
    while ((x & 1) == 0) {
        x >>= 1;
        ++i;
    }
    return i;
}

/* Test whether a number is a W‑number (definition of the sequence). */
static int is_w_number(seq_t x)
{
    /* Extract decimal digits, least significant first. */
    unsigned char digits[20];   /* enough for 64‑bit numbers */

    size_t len = 0;
    seq_t tmp = x;
    do {
        digits[len++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    } while (tmp != 0 && len < sizeof(digits));

    /* Pre‑compute digit sums for all subsets of positions. */
    unsigned int full = 1U << (unsigned int)len;
    static unsigned int rsum[1U << 20];  /* up to 20 digits */
    rsum[0] = 0;
    for (unsigned int mask = 1; mask < full; ++mask) {
        unsigned int bit = lsb_index(mask);
        unsigned int prev = mask & (mask - 1);
        rsum[mask] = rsum[prev] + digits[bit];
    }

    /* For each digit, ensure some other subset of at least two digits
       sums to it. */
    for (size_t i = 0; i < len; ++i) {
        int ok = 0;
        for (unsigned int mask = 0; mask < full; ++mask) {
            if (mask & (1U << (unsigned int)i))          continue;   /* cannot use the digit itself */
            if ((mask & (mask - 1)) == 0)               continue;   /* need at least two digits */
            if (rsum[mask] == digits[i]) { ok = 1; break; }
        }
        if (!ok) return 0;   /* all digits must satisfy condition */
    }
    return 1;
}

/* Cache of the sequence values that have been found. */
static seq_t *seq_cache = NULL;   /* array of found terms */
static size_t seq_cap  = 0;        /* allocated capacity */
static size_t seq_len  = 0;        /* number of terms stored */

/* Next number to test for W‑property. */
static seq_t next_candidate = 10000;   /* ensures the search starts at 10001 */

/* Ensure the cache can hold at least `need` elements. */
static void ensure_cache(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *newp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (seq_cache) {
        memcpy(newp, seq_cache, seq_len * sizeof(seq_t));
        free(seq_cache);
    }
    seq_cache = newp;
    seq_cap   = new_cap;
}

/* Return the nth term of A203591 (a(1)=10001).  Assumes n >= 1. */
seq_t a(idx_t n)
{
    /* Find up to n terms. */
    while (seq_len < (size_t)n) {
        /* Start from the next number after the last found. */
        ++next_candidate;   /* first iteration: from 10001 */

        /* Search for the next W‑number. */
        while (!is_w_number(next_candidate)) {
            ++next_candidate;
        }

        /* Store the found number. */
        ensure_cache(seq_len + 1);
        seq_cache[seq_len++] = next_candidate;
    }

    return seq_cache[(size_t)n - 1];
}
