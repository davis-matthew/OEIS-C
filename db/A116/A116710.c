/*
 * A116710  Number of permutations of length n which avoid the patterns 1423,
 *          3421.
 * https://oeis.org/A116710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 47*a(n-2) + 99*a(n-3) - 109*a(n-4) +
 *            63*a(n-5) - 18*a(n-6) + 2*a(n-7); Initial values (offset 1):
 *            a(1)=1, a(2)=2, a(3)=6, a(4)=22, a(5)=86, a(6)=337, a(7)=1299
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: data[0] is unused, indices 1..maxComputed are valid. */
    static seq_t *data      = NULL;   /* pointer to cached values            */
    static size_t allocated = 0;      /* allocated array size (excluding 0)  */
    static size_t maxComputed = 0;    /* highest index that has been computed */

    /* if n is zero (outside the officially defined range) return 0 */
    if (n == 0) return 0;

    /* convert n to a type that can be used for array indexing */
    const size_t req = (size_t)n;

    /* allocate and initialise the cache on first call */
    if (!data) {
        allocated   = 8;                     /* enough for indices 0..8 */
        data        = (seq_t *)malloc((allocated + 1) * sizeof(seq_t));
        if (!data) return 0;                 /* allocation failed */
        data[0]     = 0;                     /* unused */
        data[1]     = 1;
        data[2]     = 2;
        data[3]     = 6;
        data[4]     = 22;
        data[5]     = 86;
        data[6]     = 337;
        data[7]     = 1299;
        maxComputed = 7;
    }

    /* grow the allocated buffer if we need more space */
    if (req > allocated) {
        size_t newSz = allocated;
        while (req > newSz) newSz <<= 1;
        seq_t *tmp = (seq_t *)realloc(data, (newSz + 1) * sizeof(seq_t));
        if (!tmp) return 0;                  /* realloc failed */
        data      = tmp;
        allocated = newSz;
    }

    /* compute values up to the requested index */
    if (req > maxComputed) {
        for (size_t i = maxComputed + 1; i <= req; ++i) {
            /* Positive contributions */
            seq_t pos = data[i-1] * 11
                       + data[i-3] * 99
                       + data[i-5] * 63
                       + data[i-7] * 2;

            /* Negative contributions */
            seq_t neg = data[i-2] * 47
                       + data[i-4] * 109
                       + data[i-6] * 18;

            data[i] = pos - neg;   /* pos ≥ neg for all n ≥ 8 */
        }
        maxComputed = req;
    }

    return data[(size_t)n];
}
