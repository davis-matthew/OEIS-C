/*
 * A096868  Where records occur in A068189.
 * https://oeis.org/A096868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = index m of the n-th record of A068189 (search: scan
 *            m=1,2,..., compare A068189(m) as digit strings, count strict
 *            records)
 * Range:     u8 n <= 34, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static int A096868_digits(uint64_t m, unsigned char *d)
{
    if (m == 1) { d[0] = 1; return 1; }
    int e2 = 0, e3 = 0, e5 = 0, e7 = 0;
    while ((m & 1) == 0) { e2++; m >>= 1; }
    while (m % 3 == 0) { e3++; m /= 3; }
    while (m % 5 == 0) { e5++; m /= 5; }
    while (m % 7 == 0) { e7++; m /= 7; }
    if (m != 1) return 0;
    int c = 0;
    int c9 = e3 / 2; e3 -= 2 * c9;
    int c8 = e2 / 3; e2 -= 3 * c8;
    int c6 = 0;
    while (e2 > 0 && e3 > 0) { c6++; e2--; e3--; }
    int c4 = e2 / 2; e2 -= 2 * c4;
    int cnt[10] = {0};
    cnt[9] = c9; cnt[8] = c8; cnt[6] = c6; cnt[7] = e7; cnt[5] = e5;
    cnt[4] = c4; cnt[3] = e3; cnt[2] = e2;
    for (int v = 2; v <= 9; v++)
        for (int i = 0; i < cnt[v]; i++) d[c++] = (unsigned char)v;
    return c;
}

static int A096868_greater(const unsigned char *a, int la, const unsigned char *b, int lb)
{
    if (la != lb) return la > lb;
    for (int i = 0; i < la; i++)
        if (a[i] != b[i]) return a[i] > b[i];
    return 0;
}

seq_t a(idx_t n)
{
    unsigned char best[256], cur[256];
    int lb = 0;
    uint64_t count = 0;
    uint64_t m = 0;
    for (;;) {
        m++;
        int lc = A096868_digits(m, cur);
        if (lc == 0) continue;
        if (lb == 0 || A096868_greater(cur, lc, best, lb)) {
            for (int i = 0; i < lc; i++) best[i] = cur[i];
            lb = lc;
            count++;
            if (count == (uint64_t)n) return (seq_t)m;
        }
    }
}
