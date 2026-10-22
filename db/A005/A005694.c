/*
 * A005694  Positions of remoteness 6 in Beans-Don't-Talk.
 * https://oeis.org/A005694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. Beans-Don't-Talk: the move from n goes to the odd part
 *            of 3n-1 or of 3n+1, and 1 is terminal (remoteness 0). If any
 *            option is a P-position, rem(n) = 1 + min of the P-options'
 *            remoteness. If every option is an N-position, rem(n) = 1 + max
 *            of their remoteness. Positions of remoteness r are...
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128;

static u128 *a5694_res = 0;
static size_t a5694_cnt = 0;
static int a5694_bits = 0;

static u128 a5694_odd(u128 x)
{
    while (!(x & 1)) x >>= 1;
    return x;
}

/* exact remoteness if <= L, else -1 */
static int a5694_rem(u128 n, int L)
{
    int r1, r2, best = -1;
    if (n == 1) return 0;
    if (L == 0) return -1;
    r1 = a5694_rem(a5694_odd(3 * n - 1), L - 1);
    r2 = a5694_rem(a5694_odd(3 * n + 1), L - 1);
    if (r1 >= 0 && r1 % 2 == 0) best = r1;
    if (r2 >= 0 && r2 % 2 == 0 && (best < 0 || r2 < best)) best = r2;
    if (best >= 0) return best + 1;
    if (r1 >= 0 && r2 >= 0) return 1 + (r1 > r2 ? r1 : r2);
    return -1;
}

static int a5694_cmp(const void *p, const void *q)
{
    u128 x = *(const u128 *)p, y = *(const u128 *)q;
    return x < y ? -1 : (x > y ? 1 : 0);
}

static void a5694_generate(int bits)
{
    u128 B[7];
    u128 *S, *T;
    size_t sn, tn, tcap, i, k;
    int r;
    B[6] = (u128)1 << bits;
    for (r = 5; r >= 0; r--) B[r] = 3 * B[r + 1] + 1;
    S = (u128 *)malloc(sizeof(u128));
    S[0] = 1;
    sn = 1;
    for (r = 1; r <= 6; r++) {
        tcap = 64;
        tn = 0;
        T = (u128 *)malloc(tcap * sizeof(u128));
        for (i = 0; i < sn; i++) {
            u128 m = S[i], x, lim = 3 * B[r] + 1;
            if (m % 3 == 0) continue;
            for (x = m; x <= lim; x <<= 1) {
                u128 n = (x % 3 == 1) ? (x - 1) / 3 : (x + 1) / 3;
                if (n >= 2 && n <= B[r] && a5694_rem(n, r) == r) {
                    if (tn == tcap) {
                        tcap *= 2;
                        T = (u128 *)realloc(T, tcap * sizeof(u128));
                    }
                    T[tn++] = n;
                }
            }
        }
        if (tn > 1) qsort(T, tn, sizeof(u128), a5694_cmp);
        k = 0;
        for (i = 0; i < tn; i++)
            if (k == 0 || T[k - 1] != T[i]) T[k++] = T[i];
        free(S);
        S = T;
        sn = k;
    }
    free(a5694_res);
    a5694_res = S;
    a5694_cnt = sn;
    a5694_bits = bits;
}

seq_t a(idx_t n)
{
    if (a5694_bits == 0) a5694_generate(12);
    while ((idx_t)a5694_cnt < n && a5694_bits + 6 <= 110)
        a5694_generate(a5694_bits + 6);
    if ((idx_t)a5694_cnt < n) return (seq_t)0;
    return u128_to_seq(a5694_res[n - 1]);
}
