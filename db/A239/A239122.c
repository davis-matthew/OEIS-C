/*
 * A239122  Partial sums of A061019.
 * https://oeis.org/A239122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σₖ₌₁ⁿ (-1)^Ω(k) * k – partial sums of A061019
 * Range:     i8 n <= 31, i16 n <= 1614, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---- state for incremental computation ------------------------------------ */
static idx_t  computed_max = 0;   /* largest n for which cum array is valid   */
static seq_t *cum          = NULL;/* cumulative sums: cum[0] = 0, cum[n] = a(n) */
static size_t cum_cap      = 0;   /* allocated capacity of cum[]               */

static uint32_t *spf      = NULL;/* smallest prime factor for numbers ≤ recent limit */
static size_t   spf_cap   = 0;    /* allocated capacity of spf[]                */

/* ---- linear sieve for smallest prime factor up to limit ----------------- */
static void ensure_spf(idx_t limit)
{
    if ((size_t)limit < spf_cap)
        return; /* already available */

    size_t new_cap = (size_t)limit + 1;          /* include 0…limit */
    uint32_t *old = spf;
    spf = (uint32_t *)realloc(spf, new_cap * sizeof(uint32_t));
    if (!spf) {
        spf = old;              /* allocation failed – keep old data */
        return;
    }
    /* initialize, filling spf[i] = i as a provisional value */
    for (size_t i = spf_cap; i < new_cap; ++i)
        spf[i] = (uint32_t)i;

    /* classic sieve to replace spf of composites with their smallest prime factor */
    for (size_t i = 2; i * i <= (size_t)limit; ++i) {
        if (spf[i] == (uint32_t)i) {               /* i is prime */
            for (size_t j = i * i; j <= (size_t)limit; j += i) {
                if (spf[j] == (uint32_t)j)    /* not set yet */
                    spf[j] = (uint32_t)i;
            }
        }
    }
    spf_cap = new_cap;
}

/* ---- ensure that a(n) is computed up to n ------------------------------- */
static void ensure_cumulative(idx_t n)
{
    if (n <= computed_max)
        return;

    ensure_spf(n);

    size_t required = (size_t)n + 1;          /* need indices 0…n */
    if (cum_cap < required) {
        seq_t *old = cum;
        cum = (seq_t *)realloc(cum, required * sizeof(seq_t));
        if (!cum) {
            cum = old;      /* allocation failed – fall back to old data */
            return;
        }
        cum_cap = required;
    }

    cum[0] = (seq_t)0;   /* base case */

    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t  term = (seq_t)i;
        idx_t  x = i;
        int    parity = 0;               /* 0 = even Ω, 1 = odd Ω */

        /* count Ω(i) by dividing out the smallest prime factors repeatedly */
        while (x > 1) {
            uint32_t p = spf[(size_t)x];
            parity ^= 1;
            x /= p;
        }

        if (parity)
            term = -term;                /* (-1)^Ω(i) * i */

        cum[(size_t)i] = cum[(size_t)(i - 1)] + term;
    }

    computed_max = n;
}

/* ---- public entry point --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n=0 – return 0 */

    ensure_cumulative(n);
    return cum[(size_t)n];
}
