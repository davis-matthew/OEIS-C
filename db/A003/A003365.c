/*
 * A003365  Numbers that are the sum of 9 positive 6th powers.
 * https://oeis.org/A003365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- mark every sum of 9 positive 6th powers up to a limit
 *            L (multisets enumerated with pruning), doubling L until at least
 *            n terms are found; a(n) is the n-th marked value
 * Range:     u8 n <= 4, u16 n <= 776, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3365_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a3365_rec.h"

seq_t a(idx_t n) {
    static uint64_t *vals = NULL;
    static idx_t cnt = 0;
    static uint64_t lim = 1u << 15;

    while (cnt < n) {
        lim *= 2;
        unsigned char *mark = (unsigned char *)calloc((size_t)lim + 1, 1);
        a3365_rec(mark, lim, 9, 1, 0);
        idx_t c = 0;
        for (uint64_t i = 0; i <= lim; i++)
            if (mark[i]) c++;
        uint64_t *nv = (uint64_t *)malloc((size_t)(c ? c : 1) * sizeof(uint64_t));
        idx_t j = 0;
        for (uint64_t i = 0; i <= lim; i++)
            if (mark[i]) nv[j++] = i;
        free(mark);
        free(vals);
        vals = nv;
        cnt = c;
    }
    return (seq_t)vals[n - 1];
}
