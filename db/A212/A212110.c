/*
 * A212110  Number of (w,x,y,z) with all terms in {1,...,n} and w*x + y*z =
 *          n^2.
 * https://oeis.org/A212110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (w,x,y,z) with 1 ≤ w,x,y,z ≤ n and w*x + y*z =
 *            n²
 * Range:     u8 n <= 15, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>


seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t S = (seq_t)n * (seq_t)n;          /* n² */

    /* ------------------------------------------------------------------ */
    /* Cache for counts of ordered pairs (y,z) with product p = y*z        */
    /* ------------------------------------------------------------------ */
    static uint16_t *prod_counts = NULL;
    static size_t prod_size   = 0;          /* allocated size (n²+1)          */

    size_t need = (size_t)(S + 1);
    if (need > prod_size) {
        free(prod_counts);
        prod_counts = (uint16_t *)malloc(need * sizeof(uint16_t));
        prod_size   = need;
        if (!prod_counts)              /* allocation failed */
            return 0;
    }

    /* clear the array (only the part up to S) */
    memset(prod_counts, 0, need * sizeof(uint16_t));

    /* ------------------------------------------------------------------ */
    /* Count all ordered pairs (y,z) with product p = y*z                */
    /* ------------------------------------------------------------------ */
    idx_t N = n;
    for (idx_t y = 1; y <= N; ++y) {
        for (idx_t z = 1; z <= N; ++z) {
            size_t idx = (size_t)(y * z);
            prod_counts[idx] += 1;
        }
    }

    /* ------------------------------------------------------------------ */
    /* Compute a(n) = Σ_{a=1}^{S-1} count(a) * count(S-a)               */
    /* ------------------------------------------------------------------ */
    seq_t res = 0;
    for (idx_t a = 1; a < (idx_t)S; ++a) {
        uint16_t f1 = prod_counts[a];
        if (f1 == 0)
            continue;
        uint16_t f2 = prod_counts[(size_t)(S - a)];
        res += (seq_t)f1 * (seq_t)f2;
    }

    return res;
}
