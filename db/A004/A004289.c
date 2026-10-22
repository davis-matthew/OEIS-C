/*
 * A004289  Least positive multiple of n written in base 9 using only 0 and 1.
 * https://oeis.org/A004289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   BFS over residues mod n (digits 0/1 read in base 9); a(n) is the
 *            smallest such multiple, written with decimal digits 0/1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    long long *par = (long long *)malloc((size_t)N * sizeof(long long));
    unsigned char *dig = (unsigned char *)malloc((size_t)N);
    unsigned char *vis = (unsigned char *)calloc((size_t)N, 1);
    uint64_t *q = (uint64_t *)malloc((size_t)N * sizeof(uint64_t));
    unsigned char *buf;
    uint64_t head = 0, tail = 0, r0, x;
    size_t len = 0, i;
    seq_t v = 0;

    r0 = 1 % N;
    vis[r0] = 1;
    par[r0] = -1;
    dig[r0] = 1;
    q[tail++] = r0;
    while (head < tail) {
        uint64_t r = q[head++];
        int d;
        if (r == 0) break;
        for (d = 0; d <= 1; d++) {
            uint64_t s = (r * 9 + (uint64_t)d) % N;
            if (!vis[s]) {
                vis[s] = 1;
                par[s] = (long long)r;
                dig[s] = (unsigned char)d;
                q[tail++] = s;
            }
        }
    }

    buf = (unsigned char *)malloc((size_t)N + 1);
    x = 0;
    for (;;) {
        buf[len++] = dig[x];
        if (par[x] < 0) break;
        x = (uint64_t)par[x];
    }
    for (i = len; i-- > 0;) {
        v = v * 10 + (seq_t)buf[i];
    }

    free(buf);
    free(par);
    free(dig);
    free(vis);
    free(q);
    return v;
}
