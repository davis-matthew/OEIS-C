/*
 * A027748  Irregular triangle in which first row is 1, n-th row (n > 1) lists
 *          distinct prime factors of n.
 * https://oeis.org/A027748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf, nice
 * Formula:   a(n) = term of the triangle where the first row is 1 and the
 *            n-th row (n > 1) lists distinct prime factors of n. The sequence
 *            is obtained by reading each row from left to right, top to
 *            bottom
 * Range:     u8 n <= 486, u16 n <= 4569, u32 n <= 4569, u64 n <= 4569,
 *            u128 n <= 4569, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static idx_t *spf_ptr = NULL;           /* smallest prime factor table */
static idx_t spf_limit = 0;             /* current size of the table */

/* Build a smallest‑prime‑factor table up to 'limit' (inclusive). */
static void build_spf(idx_t limit, idx_t **spf_out)
{
    /* allocate memory */
    idx_t *spf = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));
    if (!spf) return; /* allocation failure – should not happen */

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    *spf_out = spf;
    *spf_out = spf; /* return via pointer */
}

/* Ensure that the SPF table covers at least 'max_needed' entries. */
static void ensure_spf(idx_t max_needed)
{
    if (spf_limit >= max_needed) return;

    /* free old table if any */
    if (spf_ptr) free(spf_ptr);

    /* create new table */
    build_spf(max_needed, &spf_ptr);
    spf_limit = max_needed;
}

seq_t a(idx_t n)
{
    /* n is 1‑based index in the flattened triangle. */
    if (n <= 1) return (seq_t)1;

    /* Estimate maximum number we need to process.
       For n <= 10000 an upper bound of 20000 is safe. */
    const idx_t ESTIMATE_LIMIT = 20000;
    ensure_spf(ESTIMATE_LIMIT);

    /* Count terms until we reach position 'n'. */
    idx_t cumulative = 0;

    for (idx_t m = 1; m <= spf_limit; ++m) {
        idx_t row_count;
        if (m == 1) {
            row_count = 1; /* row 1 contains only 1 */
        } else {
            /* count distinct prime factors of m */
            idx_t temp = m;
            idx_t last_pf = 0;
            row_count = 0;
            while (temp > 1) {
                idx_t p = spf_ptr[temp];
                if (p != last_pf) {
                    ++row_count;
                    last_pf = p;
                }
                while (temp % p == 0) temp /= p;
            }
        }

        if (cumulative + row_count >= n) {
            /* The desired term is in this row. */
            idx_t offset = n - cumulative - 1; /* 0‑based index in row */
            if (m == 1) {
                /* Only element 1 in row 1. */
                return (seq_t)1;
            }

            /* Extract the offset‑th distinct prime factor. */
            idx_t temp = m;
            idx_t last_pf = 0;
            idx_t idx = 0;
            while (temp > 1) {
                idx_t p = spf_ptr[temp];
                if (p != last_pf) {
                    if (idx == offset) return (seq_t)p;
                    ++idx;
                    last_pf = p;
                }
                while (temp % p == 0) temp /= p;
            }
        }

        cumulative += row_count;
    }

    /* If we get here the estimate was insufficient; enlarge and retry. */
    idx_t new_limit = spf_limit * 2;
    ensure_spf(new_limit);
    return a(n); /* recurse with larger table */
}
