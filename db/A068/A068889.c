/*
 * A068889  Triangular numbers with property that digits alternate in parity
 *          individually as well as in concatenation with previous terms.
 * https://oeis.org/A068889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th triangular number whose decimal digits alternate
 *            in parity, and that alternates in parity with the preceding term
 *            (when it exists)
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   last_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */
/* The build supplies the integer types seq_t and idx_t.  We
   use a static array large enough to hold the terms for n up to
   about 100 000.  This keeps the implementation simple and
   avoids the C++ void*‑to‑class conversion that broke the
   previous version. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/last_digit.h"

/* Adjust the size if you expect more than 500 000 terms. */
#define TERM_CACHE_CAP 500000

static seq_t term_cache[TERM_CACHE_CAP];
static size_t cache_len      = 0;   /* number of cached terms          */
static idx_t  last_k         = 0;   /* last triangular index already tested */
static seq_t  last_term      = 0;   /* a(cache_len) if cache_len>0     */

/* ------------------------------------------------------------ */
/* Utility: check whether the decimal digits of x alternate in
   parity.                                 */
static bool alternates(seq_t x)
{
    if (x < 10)                 /* a single digit is fine */
        return true;

    int prev_parity = -1;
    while (x > 0) {
        int digit  = (int)(x % 10);
        int parity = digit & 1;            /* 0 even, 1 odd */
        if (prev_parity != -1 && parity == prev_parity)
            return false;      /* two consecutive digits with same parity */
        prev_parity = parity;
        x /= 10;
    }
    return true;
}

/* Get the most significant (first) decimal digit of a non‑negative integer. */
static int first_digit(seq_t x)
{
    while (x >= 10)
        x /= 10;
    return (int)x;
}

/* ------------------------------------------------------------ */
/* Generate the next term that satisfies both conditions,
   updating last_k and last_term accordingly. */
static void generate_next_term(void)
{
    for (;;) {
        last_k += 1;                     /* test next triangular index */
        seq_t tri = ((seq_t)last_k) * ((seq_t)(last_k + 1)) / 2;

        if (!alternates(tri))
            continue;                     /* fail condition (1) */

        if (cache_len > 0) {              /* we already have a previous term */
            int last_par  = last_digit(last_term);
            int first_par = first_digit(tri);
            if ((last_par & 1) == (first_par & 1))
                continue;                 /* boundary parity fails */
        }

        /* All tests passed – store the term */
        if (cache_len >= TERM_CACHE_CAP) {
            /* In practice this should never happen for the tested range */
            abort();
        }
        term_cache[cache_len++] = tri;
        last_term = tri;
        break;
    }
}

/* ------------------------------------------------------------ */
/* Public function required by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                          /* undefined for offset 0, but safe */

    /* Grow cache until it contains a(n) */
    while (cache_len < n) {
        generate_next_term();
    }
    return term_cache[n - 1];
}
