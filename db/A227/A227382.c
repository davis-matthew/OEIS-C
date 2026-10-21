/*
 * A227382  Number of n X 3 0,1 arrays indicating 2 X 2 subblocks of some
 *          larger (n+1) X 4 binary array having a sum of one, with rows and
 *          columns of the latter in lexicographically nondecreasing order.
 * https://oeis.org/A227382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) - 210*a(n-4) +
 *            252*a(n-5) - 210*a(n-6) + 120*a(n-7) - 45*a(n-8) + 10*a(n-9) -
 *            a(n-10) for n > 13 Initial values (n = 1 … 13): 4, 15, 54, 185,
 *            587, 1704, 4532, 11126, 25430, 54568, 110768, 214130, 396492
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 40, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle trivial or very small indices */
    if (n == 0) return (seq_t)0;
    if (n <= 13) {
        static const seq_t init[14] = {
            0ULL, /* unused for 0 */
            4ULL, 15ULL, 54ULL, 185ULL, 587ULL, 1704ULL,
            4532ULL, 11126ULL, 25430ULL, 54568ULL,
            110768ULL, 214130ULL, 396492ULL
        };
        return init[(size_t)n];
    }

    /* Allocate an array to hold all values up to n */
    size_t len = (size_t)n + 1; /* index 0 unused */
    seq_t *arr = (seq_t*)malloc(len * sizeof(seq_t));
    if (!arr) abort();

    arr[0] = (seq_t)0;
    /* Load the first 13 known values */
    static const seq_t init[14] = {
        0ULL, /* unused for 0 */
        4ULL, 15ULL, 54ULL, 185ULL, 587ULL, 1704ULL,
        4532ULL, 11126ULL, 25430ULL, 54568ULL,
        110768ULL, 214130ULL, 396492ULL
    };
    for (size_t i = 1; i <= 13; ++i)
        arr[i] = init[i];

    /* Recurrence coefficients (alternating binomial coefficients of (1-1)^10) */
    static const int coeff[10] = {
        10, -45, 120, -210, 252, -210, 120, -45, 10, -1
    };

    /* Compute a(n) for n > 13 */
    for (size_t i = 14; i <= (size_t)n; ++i) {
        __int128 sum = 0;
        for (int k = 1; k <= 10; ++k) {
            sum += (__int128)coeff[k - 1] * (__int128)arr[i - k];
        }
        arr[i] = (seq_t)sum;
    }

    seq_t result = arr[(size_t)n];
    free(arr);
    return result;
}
