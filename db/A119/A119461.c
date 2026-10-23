/*
 * A119461  Zero-free numbers with digit sum equal to 7.
 * https://oeis.org/A119461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 57, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A119461: Zero‐free numbers with digit sum equal to 7.
   The sequence is finite: there are exactly 64 such numbers, the
   largest being 1111111.  The function enumerates all such numbers
   once and returns the n‑th term (a(1)=7).  For n > 64 the function
   returns 0. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Return 0 for illegal offset 0. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache for the generated terms. */
    static seq_t *cache = NULL;
    static size_t size = 0;
    static int built = 0;

    if (!built) {
        /* Maximum value to search: 7-digit number composed of 1's. */
        const idx_t max_search = 1111111;
        /* Upper bound on number of terms: 64. */
        cache = (seq_t *)malloc(64 * sizeof(seq_t));
        size = 0;

        for (idx_t i = 1; i <= max_search; ++i) {
            idx_t x = i;
            int sum = 0;
            int zero = 0;
            while (x > 0) {
                int d = x % 10;
                if (d == 0) {
                    zero = 1;
                    break;
                }
                sum += d;
                x /= 10;
            }
            if (!zero && sum == 7) {
                cache[size++] = (seq_t)i;
            }
        }
        built = 1;
    }

    if (n - 1 < size)
        return cache[n - 1];
    else
        return (seq_t)0;   /* Undefined beyond the 64th term. */
}
