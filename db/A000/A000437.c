/*
 * A000437  Smallest nonnegative number that is the sum of 3 squares in
 *          exactly n ways.
 * https://oeis.org/A000437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve search: count representations m = x^2+y^2+z^2 with
 *            0<=x<=y<=z for all m <= L (doubling L as needed); a(n) =
 *            smallest m with exactly n ways
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *a437_first = NULL;   /* first[k] = smallest m with k ways, or UINT64_MAX */
static uint64_t a437_cap = 0;         /* size of first[] */
static uint64_t a437_L = 0;           /* sieve limit processed so far */

static void a437_sieve(uint64_t L) {
    uint32_t *cnt = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    uint64_t x, y, z, m, maxk = 0;
    if (!cnt) return;
    for (x = 0; 3 * x * x <= L; x++) {
        uint64_t sx = x * x;
        for (y = x; sx + 2 * y * y <= L; y++) {
            uint64_t sxy = sx + y * y;
            uint64_t s = sxy + y * y;
            for (z = y; s <= L; z++) {
                cnt[s]++;
                s += 2 * z + 1;
            }
        }
    }
    for (m = 0; m <= L; m++)
        if (cnt[m] > maxk) maxk = cnt[m];
    if (maxk + 1 > a437_cap) {
        uint64_t nc = maxk + 1, i;
        uint64_t *nf = (uint64_t *)malloc((size_t)nc * sizeof(uint64_t));
        for (i = 0; i < nc; i++) nf[i] = UINT64_MAX;
        if (a437_first) {
            for (i = 0; i < a437_cap; i++) nf[i] = a437_first[i];
            free(a437_first);
        }
        a437_first = nf;
        a437_cap = nc;
    }
    for (m = 0; m <= L; m++) {
        uint32_t k = cnt[m];
        if (a437_first[k] == UINT64_MAX || a437_first[k] > m) a437_first[k] = m;
    }
    free(cnt);
    a437_L = L;
}

seq_t a(idx_t n) {
    uint64_t L;
    while (1) {
        if (a437_first && (uint64_t)n < a437_cap && a437_first[n] != UINT64_MAX)
            return (seq_t)a437_first[n];
        L = a437_L ? a437_L * 2 : 4096;
        a437_sieve(L);
    }
}
