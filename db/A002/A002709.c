/*
 * A002709  Triangulations of the disk G_{n,0}.
 * https://oeis.org/A002709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A000260(n) + 2*F(n))/3, where A000260(n) =
 *            6*(4n+1)!/(n!*(3n+3)!) counts rooted triangulations (Brown), and
 *            F(n) counts the triangulations with 3-fold rotational symmetry:
 *            F(3k) = binomial(4k,k)/(3k+1), F(3k+1) =
 *            binomial(4k+2,k)/(2k+1), F(3k+2) = 0. (Burnside's lemma under
 *            rotation of th...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 24, u128 n <= 44,
 *            big any n
 * Helpers:   a2709_add_fact, a2709_add_int, a2709_div3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/a2709_add_fact.h"
#include "../utils/a2709_add_int.h"
#include "../utils/a2709_div3.h"

seq_t a(idx_t n)
{
    idx_t N = 4 * n + 4;
    unsigned char *comp = (unsigned char *)calloc(N + 1, 1);
    long *e = (long *)calloc(N + 1, sizeof(long));
    for (idx_t i = 2; i * i <= N; i++)
        if (!comp[i])
            for (idx_t j = i * i; j <= N; j += i) comp[j] = 1;

    /* R = 6*(4n+1)!/(n!(3n+3)!) */
    a2709_add_int(e, 6, 1);
    a2709_add_fact(e, comp, 4 * n + 1, 1);
    a2709_add_fact(e, comp, n, -1);
    a2709_add_fact(e, comp, 3 * n + 3, -1);
    uint64_t rR = 0, rF = 0;
    seq_t result = a2709_div3(e, N, &rR);

    if (n % 3 != 2) {
        for (idx_t i = 0; i <= N; i++) e[i] = 0;
        idx_t k = n / 3;
        a2709_add_int(e, 2, 1);
        if (n % 3 == 0) {
            a2709_add_fact(e, comp, 4 * k, 1);
            a2709_add_fact(e, comp, k, -1);
            a2709_add_fact(e, comp, 3 * k + 1, -1);
        } else {
            a2709_add_fact(e, comp, 4 * k + 2, 1);
            a2709_add_fact(e, comp, k, -1);
            a2709_add_fact(e, comp, 3 * k + 2, -1);
            a2709_add_int(e, 2 * k + 1, -1);
        }
        result += a2709_div3(e, N, &rF);
    }
    result += (rR + rF) / 3;

    free(comp);
    free(e);
    return result;
}
