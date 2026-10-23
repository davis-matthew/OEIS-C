/*
 * A022550  Numbers that are not the sum of a square and a nonnegative cube.
 * https://oeis.org/A022550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 175, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A022550: Numbers that are NOT the sum of a square and a nonnegative cube.
   We sieve all sums i^2 + j^3 up to a limit large enough to contain the
   n‑th missing number, then scan for the n‑th unmarked integer. */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* offset is 1; return 0 for n=0 */
        return (seq_t)0;

    size_t limit = (size_t)(n * 3);
    if (limit < 10)            /* small safety net for tiny n   */
        limit = 10;

    while (1) {
        unsigned char *rep = (unsigned char *)malloc((limit + 1) * sizeof(unsigned char));
        if (!rep)              /* allocation failed */
            return (seq_t)0;

        memset(rep, 0, limit + 1);

        /* Mark every number that can be expressed as i^2 + j^3. */
        for (size_t i = 0; i * i <= limit; ++i) {
            size_t sq = i * i;
            for (size_t j = 0; ; ++j) {
                size_t cube = j * j * j;
                size_t v = sq + cube;
                if (v > limit)
                    break;
                rep[v] = 1;
            }
        }

        /* Find the n-th missing number. */
        size_t missing = 0;
        size_t result = 0;
        for (size_t x = 1; x <= limit; ++x) {
            if (!rep[x]) {
                ++missing;
                if (missing == n) {
                    result = x;
                    break;
                }
            }
        }

        free(rep);

        if (missing == n)          /* found it */
            return (seq_t)result;

        /* If not enough numbers were found, double the search limit and try again. */
        limit <<= 1;                /* limit = limit * 2 */
    }
}
