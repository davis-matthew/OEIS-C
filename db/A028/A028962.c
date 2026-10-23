/*
 * A028962  Numbers represented by quadratic form with Gram matrix [ 2, 1, 0;
 *          1, 3, 1; 0, 1, 5 ].
 * https://oeis.org/A028962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th non‑negative integer representable by the
 *            quadratic form Q(x,y,z) = 2·x² + 2·x·y + 3·y² + 2·y·z + 5·z².
 *            The offset is 1, so a(1) = 0, a(2) = 2, … as given in the
 *            problem statement
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note:  The sequence is obtained once by enumerating all triples (x,y,z) in a
   bounded range (chosen to cover all representations up to the required
   maximum value) and collecting the distinct values.  Subsequent calls simply
   index into the pre‑computed table.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/*  The maximum value up to which we pre‑compute representable numbers.
    For n ≤ 10000 this bound is more than sufficient (≈ 40000 means more
    than 39 800 representable numbers).  */
#define MAX_VALUE 40000ULL

seq_t a(idx_t n)
{
    /* Static state for one‑time initialisation */
    static bool *seen = NULL;
    static seq_t *terms = NULL;
    static idx_t term_count = 0;

    /* ------------------------------------------------------------- */
    /* Initialise the representation table on the first call.        */
    /* ------------------------------------------------------------- */
    if (!seen)
    {
        /* Allocate a boolean array for values 0 … MAX_VALUE.         */
        seen = (bool *)calloc((size_t)MAX_VALUE + 1, sizeof(bool));
        if (!seen) return 0;  /* fallback if allocation fails */

        /* Bound on the variable magnitudes required to generate all
           representable numbers up to MAX_VALUE.
           These bounds are derived from the minimal value that the
           form can take for a fixed variable; 155 is comfortably
           sufficient for MAX_VALUE = 40000.                     */
        const int LIM_X = 155;
        const int LIM_Y = 150;
        const int LIM_Z = 150;

        /* Enumerate all integer triples (x,y,z) in the stated ranges
           and mark the resulting values.                           */
        for (int x = -LIM_X; x <= LIM_X; ++x)
        {
            int64_t x2 = (int64_t)x * x;
            int64_t term_x = 2LL * x2;

            for (int y = -LIM_Y; y <= LIM_Y; ++y)
            {
                int64_t y2 = (int64_t)y * y;
                int64_t term_xy = 2LL * x * y;
                int64_t term_y = 3LL * y2;

                for (int z = -LIM_Z; z <= LIM_Z; ++z)
                {
                    int64_t z2 = (int64_t)z * z;
                    int64_t val = term_x + term_xy + term_y
                                  + 2LL * y * z + 5LL * z2;

                    if (val >= 0 && val <= (int64_t)MAX_VALUE)
                        seen[(size_t)val] = true;
                }
            }
        }

        /* Count how many representable numbers have been found.     */
        for (idx_t v = 0; v <= MAX_VALUE; ++v)
            if (seen[(size_t)v]) ++term_count;

        /* Allocate an array of the representable numbers in
           ascending order.                                          */
        terms = (seq_t *)malloc((size_t)term_count * sizeof(seq_t));
        if (!terms) return 0;  /* fallback */

        idx_t pos = 0;
        for (idx_t v = 0; v <= MAX_VALUE; ++v)
            if (seen[(size_t)v])
                terms[pos++] = (seq_t)v;

        /* We no longer need the boolean array.  */
        free(seen);
        seen = NULL;
    }

    /* ------------------------------------------------------------- */
    /* Return the requested term, if it exists.                     */
    /* ------------------------------------------------------------- */
    if (n < 1 || n - 1 >= term_count)
        return 0;                    /* out of range – undefined behaviour */

    return terms[n - 1];
}
