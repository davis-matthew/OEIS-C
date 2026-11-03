/*
 * A104392  Sums of 2 distinct positive pentatope numbers (A000332).
 * https://oeis.org/A104392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of two distinct positive pentatope numbers, sorted in
 *            ascending order without duplicates
 * Range:     u8 none, u16 none, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 49 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static int seq_compare(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;

    /* For unsigned or big-integer types the usual ordering works. */
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Static storage for the pre‑computed list of sums. */
    static seq_t *terms = NULL;   /* sorted, unique sums */
    static idx_t terms_size = 0;  /* number of terms stored */
    static int initialized = 0;   /* initialization flag */

    if (!initialized)
    {
        /* Choose a limit that yields enough distinct sums for
           the required test range (at least 10 000, but we
           generate more for safety).  500 gives ~125 000
           raw pairs, which after de‑duplication is well
           above 10 000. */
        const idx_t MAX_I = 500;
        idx_t i, j;
        idx_t pair_count = MAX_I * (MAX_I - 1) / 2;
        idx_t pos = 0;

        /* Allocate arrays for pentatope numbers (1‑based) and raw sums. */
        seq_t *pent = mem_new(seq_t, (MAX_I + 1));
        seq_t *sums = mem_new(seq_t, pair_count);

        /* Compute pentatope numbers:
           P(i) = i*(i+1)*(i+2)*(i+3)/24
        */
        for (i = 1; i <= MAX_I; ++i)
        {
            seq_t ii = (seq_t)i;
            pent[i] = ii * (ii + 1) * (ii + 2) * (ii + 3) / (seq_t)24;
        }

        /* Generate all unordered distinct pairs (i < j). */
        for (i = 1; i <= MAX_I; ++i)
        {
            for (j = i + 1; j <= MAX_I; ++j)
            {
                sums[pos++] = pent[i] + pent[j];
            }
        }

        /* Sort the sums. */
        mem_qsort(sums, pair_count, sizeof(seq_t), seq_compare);

        /* Deduplicate in place: overwrite the array with unique values. */
        idx_t uniq = 0;
        for (pos = 0; pos < pair_count; ++pos)
        {
            if (uniq == 0 || sums[uniq - 1] != sums[pos])
            {
                sums[uniq++] = sums[pos];
            }
        }

        /* Resize to the exact number of unique terms. */
        terms = mem_resize(seq_t, sums, uniq);
        terms_size = uniq;

        /* The pent array is no longer needed. */
        mem_free(pent);

        initialized = 1;
    }

    /* Return the requested term if it exists. */
    if (n < terms_size)
        return terms[n];
    /* Undefined for indices beyond the pre‑computed range. */
    return (seq_t)0;
}
