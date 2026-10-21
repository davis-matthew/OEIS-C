/*
 * A090657  Triangle read by rows: T(n,k) = number of functions from
 *          [1,2,...,n] to [1,2,...,n] such that the image contains exactly k
 *          elements (0<=k<=n).
 * https://oeis.org/A090657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) = C(r,k) * k! * S(r,k) r = largest
 *            integer with r(r+1)/2 <= n, k = n - r(r+1)/2 (C = binomial
 *            coefficient, S = Stirling number of the second kind)
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 70, u64 n <= 160,
 *            u128 n <= 419, big any n
 * Helpers:   index_to_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/index_to_coords.h"

#define MAX_ROW 200   /* sufficient for n up to 10000 */

static seq_t binom_table[MAX_ROW + 1][MAX_ROW + 1];
static seq_t stirling_table[MAX_ROW + 1][MAX_ROW + 1];
static seq_t fact_table[MAX_ROW + 1];
static idx_t  computed_up_to = 0;     /* index of the highest computed row */
static int    initialized = 0;        /* 0 → tables not initialized yet */

/* Initialise tables for the base case n = 0 */
static void init_tables(void)
{
    if (initialized)
        return;

    binom_table[0][0]   = (seq_t)1;
    stirling_table[0][0] = (seq_t)1;
    fact_table[0] = (seq_t)1;
    computed_up_to = 0;
    initialized = 1;
}

/* Pre‑compute tables up to row "up_to" (inclusive) */
static void prepare_tables(idx_t up_to)
{
    if (!initialized)
        init_tables();

    if (up_to <= computed_up_to)
        return;

    if (up_to > MAX_ROW)
        up_to = MAX_ROW; /* safety, though input n will not exceed this */

    for (idx_t n = computed_up_to + 1; n <= up_to; ++n)
    {
        /* factorial */
        fact_table[(size_t)n] = fact_table[(size_t)(n - 1)] * (seq_t)n;

        for (idx_t k = 0; k <= n; ++k)
        {
            /* binomial coefficient */
            if (k == 0 || k == n)
                binom_table[(size_t)n][(size_t)k] = (seq_t)1;
            else
                binom_table[(size_t)n][(size_t)k] =
                    binom_table[(size_t)(n - 1)][(size_t)(k - 1)] +
                    binom_table[(size_t)(n - 1)][(size_t)k];

            /* Stirling numbers of the second kind */
            if (k == 0)
                stirling_table[(size_t)n][(size_t)k] = (seq_t)0;
            else
                stirling_table[(size_t)n][(size_t)k] =
                    stirling_table[(size_t)(n - 1)][(size_t)(k - 1)] +
                    (seq_t)k * stirling_table[(size_t)(n - 1)][(size_t)k];
        }
    }

    computed_up_to = up_to;
}

seq_t a(idx_t n)
{
    idx_t r, k;
    index_to_coords(n, &r, &k);

    prepare_tables(r);

    /* T(r,k) = C(r,k) * k! * S(r,k) */
    return binom_table[(size_t)r][(size_t)k] *
           fact_table[(size_t)k] *
           stirling_table[(size_t)r][(size_t)k];
}
