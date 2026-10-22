/*
 * A004072  Read the terms of A004071 backwards.
 * https://oeis.org/A004072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   build the list T(n) of A004071. Start with T(0) = [1]. To get
 *            T(k) from T(k-1), replace each entry d with 1,2,...,d, except
 *            the last entry d, which becomes 1,2,...,d-1,d+1. a(n) is the
 *            concatenation of the entries of T(n) in reverse order
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    size_t len = 1;
    int *t = (int *)malloc(sizeof(int));
    t[0] = 1;
    for (idx_t k = 1; k <= n; k++) {
        size_t nl = 0;
        for (size_t i = 0; i < len; i++) nl += (size_t)t[i];
        int *u = (int *)malloc(nl * sizeof(int));
        size_t p = 0;
        for (size_t i = 0; i < len; i++) {
            int d = t[i];
            if (i + 1 < len) {
                for (int j = 1; j <= d; j++) u[p++] = j;
            } else {
                for (int j = 1; j < d; j++) u[p++] = j;
                u[p++] = d + 1;
            }
        }
        free(t);
        t = u;
        len = nl;
    }
    seq_t v = 0;
    for (size_t i = len; i-- > 0;) {
        int x = t[i];
        int m = 10;
        while (x >= m) m *= 10;
        v = v * m + x;
    }
    free(t);
    return v;
}
