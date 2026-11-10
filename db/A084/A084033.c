/*
 * A084033  Triangle read by rows: T(n, k) = A084029(n, k)/n.
 * https://oeis.org/A084033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy, tabl
 * Formula:   a(n) = T(r,k)/r, where T(r,k) = A084029(r,k) is the k-th r-digit
 *            multiple of r with digit sum r (0 if none), n = r(r-1)/2 + k.
 *            Computed by a digit-DP reachability table plus DFS in increasing
 *            order, then a long division by r
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    int r;
    int p;
    int found;
    unsigned char *reach;
    int *w;
    int *dig;
} A084033_ctx;

static int A084033_dfs(A084033_ctx *c, int pos, int cs, int cm)
{
    int r = c->r;
    if (pos < 0) {
        if (c->found == c->p) return 1;
        c->found++;
        return 0;
    }
    for (int d = (pos == r - 1) ? 1 : 0; d <= 9; ++d) {
        int ns = cs + d;
        if (ns > r) break;
        int nm = (cm + d * c->w[pos]) % r;
        int need = (r - nm) % r;
        if (!c->reach[((size_t)pos * (r + 1) + (r - ns)) * r + need]) continue;
        c->dig[pos] = d;
        if (A084033_dfs(c, pos - 1, ns, nm)) return 1;
    }
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;
    idx_t p = n - (r - 1) * r / 2 - 1;
    int R = (int)r;

    A084033_ctx c;
    c.r = R;
    c.p = (int)p;
    c.found = 0;
    size_t sz = (size_t)(R + 1) * (R + 1) * R;
    c.reach = (unsigned char *)calloc(sz, 1);
    c.w = (int *)malloc(sizeof(int) * (R + 1));
    c.dig = (int *)calloc(R + 1, sizeof(int));

    c.w[0] = 1 % R;
    for (int j = 1; j <= R; ++j) c.w[j] = (c.w[j - 1] * 10) % R;

    c.reach[0] = 1; /* i=0, s=0, m=0 */
    for (int i = 0; i < R; ++i) {
        for (int s = 0; s <= R; ++s) {
            for (int m = 0; m < R; ++m) {
                unsigned char ok = 0;
                for (int d = 0; d <= 9 && d <= s && !ok; ++d) {
                    int pm = ((m - d * c.w[i]) % R + R) % R;
                    if (c.reach[((size_t)i * (R + 1) + (s - d)) * R + pm]) ok = 1;
                }
                c.reach[((size_t)(i + 1) * (R + 1) + s) * R + m] = ok;
            }
        }
    }

    seq_t result = 0;
    if (A084033_dfs(&c, R - 1, 0, 0)) {
        int rem = 0;
        seq_t q = 0;
        for (int pos = R - 1; pos >= 0; --pos) {
            rem = rem * 10 + c.dig[pos];
            q = q * 10 + (seq_t)(rem / R);
            rem %= R;
        }
        result = q;
    }
    free(c.reach);
    free(c.w);
    free(c.dig);
    return result;
}
