/*
 * A005355  Number of asymmetric permutation rooted trees with n nodes.
 * https://oeis.org/A005355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, eigen
 * Formula:   a(0) = 0, a(1) = 1, a(n+1) = [x^n] (1 - A(x^2))/(1 - A(x)),
 *            where A(x) = Sum a(k) x^k. With B(x) = 1/(1 - A(x)): b(0) = 1,
 *            b(m) = Sum_{k=1..m} a(k)*b(m-k), and a(m+1) = b(m) - Sum_{k>=1,
 *            2k<=m} a(k)*b(m-2k)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 39, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAXN 10010

static seq_t Aa[MAXN];
static seq_t Bb[MAXN];
static idx_t cnt = 0; /* Aa[0..cnt-1] are known, Bb[0..cnt-2] are known */

seq_t a(idx_t n) {
    if (cnt == 0) {
        Aa[0] = 0;
        Aa[1] = 1;
        Bb[0] = 1;
        cnt = 2;
    }
    if (n >= MAXN) return 0;
    while (cnt <= n) {
        idx_t m = cnt;
        idx_t r = m - 1;
        idx_t k;
        seq_t s = 0;
        seq_t t;
        for (k = 1; k <= r; k++)
            s += Aa[k] * Bb[r - k];
        Bb[r] = s;
        t = s;
        for (k = 1; 2 * k <= r; k++)
            t -= Aa[k] * Bb[r - 2 * k];
        Aa[m] = t;
        cnt++;
    }
    return Aa[n];
}
