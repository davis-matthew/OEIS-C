/*
 * A166679  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A166679
   a(n) = 31*(a(n-1)+a(n-2)+…+a(n-11)) - 496*a(n-12)  for n >= 13
   a(0) = 1, a(1) = 33, a(2) = 1056, a(3) = 33792,
   a(4) = 1081344, a(5) = 34603008, a(6) = 1107296256,
   a(7) = 35433480192, a(8) = 1133871366144,
   a(9) = 36283883716608, a(10) = 1161084278931456,
   a(11) = 37154696925806592, a(12) = 1188950301625810416.
   All earlier terms come from the given recurrence. */
#include <stdlib.h>

static const seq_t init[13] = {
    1ULL,
    33ULL,
    1056ULL,
    33792ULL,
    1081344ULL,
    34603008ULL,
    1107296256ULL,
    35433480192ULL,
    1133871366144ULL,
    36283883716608ULL,
    1161084278931456ULL,
    37154696925806592ULL,
    1188950301625810416ULL
};

seq_t a(idx_t n)
{
    if (n <= 12)
        return init[(size_t)n];

    /* Allocate array with space for all needed values */
    seq_t *cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!cache)
        return 0;  /* allocation failure */

    /* Initialize known terms */
    for (size_t i = 0; i <= 12; ++i)
        cache[i] = init[i];

    /* Compute subsequent terms using the recurrence */
    for (idx_t i = 13; i <= n; ++i) {
        seq_t sum11 = 0;
        for (int j = 1; j <= 11; ++j)
            sum11 += cache[(size_t)(i - j)];
        cache[(size_t)i] = sum11 * (seq_t)31 - cache[(size_t)(i - 12)] * (seq_t)496;
    }

    seq_t result = cache[(size_t)n];
    free(cache);
    return result;
}
