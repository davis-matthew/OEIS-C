/*
 * A005241  N-positions in Epstein's Put or Take a Square game.
 * https://oeis.org/A005241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   retrograde game analysis. From x, a move goes to x - s^2 or x +
 *            s^2, where s^2 is the largest square <= x. Reaching 0 wins. a(n)
 *            is the n-th x >= 1 that is an N-position (a win for the player
 *            to move). Positions are analysed up to a bound M, and only
 *            labels well below M are used
 * Range:     u8 n <= 43, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static long long *nlist = NULL;
static long long nlen = 0;
static long long curM = 0;

static void analyse(long long M) {
    unsigned char *st = (unsigned char *)calloc((size_t)(M + 1), 1); /* 0 unknown, 1 N, 2 P */
    int *cnt = (int *)malloc(sizeof(int) * (size_t)(M + 1));
    int *deg = (int *)calloc((size_t)(M + 2), sizeof(int));
    int *radj = (int *)malloc(sizeof(int) * (size_t)(2 * M + 2));
    int *q = (int *)malloc(sizeof(int) * (size_t)(M + 1));
    long long x, r = 0, head = 0, tail = 0;

    /* out-degree (including moves beyond M, which stay unknown) and in-degree */
    cnt[0] = 0;
    for (x = 1; x <= M; x++) {
        long long s;
        while ((r + 1) * (r + 1) <= x) r++;
        s = r * r;
        cnt[x] = 2;
        deg[x - s]++;
        if (x + s <= M) deg[x + s]++;
    }
    /* prefix sums -> CSR offsets */
    {
        long long acc = 0;
        for (x = 0; x <= M + 1; x++) {
            long long d = deg[x];
            deg[x] = (int)acc;
            acc += d;
        }
    }
    {
        int *pos = (int *)malloc(sizeof(int) * (size_t)(M + 1));
        memcpy(pos, deg, sizeof(int) * (size_t)(M + 1));
        r = 0;
        for (x = 1; x <= M; x++) {
            long long s;
            while ((r + 1) * (r + 1) <= x) r++;
            s = r * r;
            radj[pos[x - s]++] = (int)x;
            if (x + s <= M) radj[pos[x + s]++] = (int)x;
        }
        free(pos);
    }

    st[0] = 2;
    q[tail++] = 0;
    while (head < tail) {
        long long v = q[head++];
        long long i;
        for (i = deg[v]; i < deg[v + 1]; i++) {
            long long y = radj[i];
            if (st[y]) continue;
            if (st[v] == 2) {
                st[y] = 1;
                q[tail++] = (int)y;
            } else if (--cnt[y] == 0) {
                st[y] = 2;
                q[tail++] = (int)y;
            }
        }
    }

    free(nlist);
    nlist = (long long *)malloc(sizeof(long long) * (size_t)(M / 64 + 1));
    nlen = 0;
    for (x = 1; x <= M / 64; x++)
        if (st[x] == 1) nlist[nlen++] = x;
    free(st);
    free(cnt);
    free(deg);
    free(radj);
    free(q);
    curM = M;
}

seq_t a(idx_t n) {
    if (curM == 0) analyse(1LL << 20);
    while ((long long)n > nlen) analyse(curM * 4);
    return (seq_t)(idx_t)nlist[n - 1];
}
