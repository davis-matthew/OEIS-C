/*
 * A005036  Number of nonequivalent dissections of a polygon into n
 *          quadrilaterals by nonintersecting diagonals up to rotation and
 *          reflection.
 * https://oeis.org/A005036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  core, nonn, nice
 * Formula:   a(n) = (2*A001764(n)/(n+1) + 6*[n even]*A001764(n/2) + 5*[n
 *            odd]*A006013((n-1)/2) + 2*[n == 1 mod 4]*A001764((n-1)/4)) / 8,
 *            i.e. a(n) = (2*(3n)!/((2n+1)!(n+1)!) + 6*[n
 *            even]*(3k)!/(k!(2k+1)!) |k=n/2 + 5*[n
 *            odd]*(3k+1)!/((2k+1)!(k+1)!) |k=(n-1)/2 + 2*[n==1 mod
 *            4]*(3m)!/(m!(2m+1)!) |m=(n-1)/4) / 8,...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 29, u128 n <= 53,
 *            big any n
 * Helpers:   term8, vp_fact, vp_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/term8.h"
#include "../utils/vp_fact.h"
#include "../utils/vp_small.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;
    uint64_t lim = 3 * (uint64_t)n + 2;
    if (lim < 7) lim = 7;
    unsigned char *comp = (unsigned char *)malloc((size_t)(lim + 1));
    memset(comp, 0, (size_t)(lim + 1));
    comp[0] = comp[1] = 1;
    for (uint64_t i = 2; i * i <= lim; i++)
        if (!comp[i])
            for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;

    seq_t total = 0, q;
    uint64_t low = 0, l;

    term8(2, 3 * n, 2 * n + 1, n + 1, comp, &q, &l);
    total += q; low += l;

    if (n % 2 == 0) {
        uint64_t k = n / 2;
        term8(6, 3 * k, k, 2 * k + 1, comp, &q, &l);
        total += q; low += l;
    } else {
        uint64_t k = (n - 1) / 2;
        term8(5, 3 * k + 1, 2 * k + 1, k + 1, comp, &q, &l);
        total += q; low += l;
    }
    if ((n - 1) % 4 == 0) {
        uint64_t m = (n - 1) / 4;
        term8(2, 3 * m, m, 2 * m + 1, comp, &q, &l);
        total += q; low += l;
    }
    free(comp);
    total += (seq_t)(low / 8);
    return total;
}
