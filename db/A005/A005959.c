/*
 * A005959  Number of acyclic ethylene derivatives with n carbon atoms.
 * https://oeis.org/A005959
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^(n-2)] (B(x)^4 + 3*B(x^2)^2)/4, where B(x) = 1 +
 *            x*(B(x)^3 + 2*B(x^3))/3 is the g.f. of A000625. It is computed
 *            without division as C^2 + P*Q, where C = B(x^2), P = (B^2 - C)/2
 *            and Q = P + C
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 24, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *Bc = NULL, *S2 = NULL, *S3 = NULL;
static idx_t blen = 0, bcap = 0;

static void ensure(idx_t N)
{
    if (N + 1 > bcap) {
        idx_t nc = bcap ? bcap : 16;
        while (nc < N + 2) nc *= 2;
        Bc = grow_zero(Bc, bcap, nc);
        S2 = grow_zero(S2, bcap, nc);
        S3 = grow_zero(S3, bcap, nc);
        bcap = nc;
    }
    if (blen == 0) { Bc[0] = 1; blen = 1; }
    while (blen <= N) {
        idx_t m = blen - 1, i;
        seq_t s2 = 0, s3 = 0;
        for (i = 0; i <= m; i++) s2 += Bc[i] * Bc[m - i];
        S2[m] = s2;
        for (i = 0; i <= m; i++) s3 += S2[i] * Bc[m - i];
        S3[m] = s3;
        if (m % 3 == 0) s3 += 2 * Bc[m / 3];
        Bc[m + 1] = s3 / 3;
        blen++;
    }
}

static seq_t cC(idx_t k) { return (k % 2 == 0) ? Bc[k / 2] : (seq_t)0; }
static seq_t cP(idx_t k) { return (S2[k] - cC(k)) / 2; }

seq_t a(idx_t n)
{
    idx_t m, i;
    seq_t r = 0;
    if (n < 2) return 0;
    m = n - 2;
    ensure(m + 1);
    /* S2 valid up to index blen-2 >= m */
    for (i = 0; i <= m; i++) {
        seq_t ci = cC(i), cj = cC(m - i);
        seq_t p = cP(i);
        seq_t q = cP(m - i) + cj;
        r += ci * cj + p * q;
    }
    return r;
}
