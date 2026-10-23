/*
 * A025395  Numbers that are the sum of 3 positive cubes in exactly 1 way.
 * https://oeis.org/A025395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: list of numbers that are the sum of three positive cubes
   in exactly one way (unordered triple, 1 ≤ x ≤ y ≤ z).  The function
   builds the list only once, then returns the n‑th element (1‑based). */

#include <stdint.h>
#include <stdlib.h>

/* Maximum sum considered; enough to contain the first ten thousand terms. */
static const idx_t LIMIT_SUM = 5000000ULL;

/* Static state */
static int initialized = 0;
static seq_t *list = NULL;         /* array of unique sums */
static size_t list_len = 0;        /* number of elements in list */

/* Helper to allocate frequencies */
static unsigned short *allocate_freq(void)
{
    /* +1 because we index from 1 to LIMIT_SUM inclusive */
    return (unsigned short *)calloc((size_t)(LIMIT_SUM + 1), sizeof(unsigned short));
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n = 0 */

    if (!initialized)
    {
        /* --- 1. Count representations of each sum -------------------------------- */
        unsigned short *freq = allocate_freq();
        if (!freq) return 0;         /* allocation failure */

        for (idx_t x = 1; ; ++x)
        {
            uint64_t x3 = (uint64_t)x * x * x;
            if (x3 > LIMIT_SUM) break;

            for (idx_t y = x; ; ++y)
            {
                uint64_t y3 = (uint64_t)y * y * y;
                uint64_t sum_xy = x3 + y3;
                if (sum_xy > LIMIT_SUM) break;

                for (idx_t z = y; ; ++z)
                {
                    uint64_t z3 = (uint64_t)z * z * z;
                    uint64_t s = sum_xy + z3;
                    if (s > LIMIT_SUM) break;

                    ++freq[(size_t)s];
                }
            }
        }

        /* --- 2. Determine number of unique sums ----------------------------------- */
        for (size_t i = 1; i <= LIMIT_SUM; ++i)
        {
            if (freq[i] == 1) ++list_len;
        }

        list = (seq_t *)malloc(list_len * sizeof(seq_t));
        if (!list)
        {
            free(freq);
            return 0;                 /* allocation failure */
        }

        /* --- 3. Store the unique sums in increasing order ------------------------ */
        size_t pos = 0;
        for (size_t i = 1; i <= LIMIT_SUM && pos < list_len; ++i)
        {
            if (freq[i] == 1)
                list[pos++] = (seq_t)i;
        }

        free(freq);
        initialized = 1;
    }

    /* --- 4. Return the requested term ------------------------------------------- */
    if ((size_t)n > list_len) return 0;   /* out of range */

    return list[n - 1];
}
