/*
 * A000632  Number of esters with n carbon atoms up to structural isomerism.
 * https://oeis.org/A000632
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. A(x) = x*B(x)*(B(x)-1), so a(n) = Sum_{i=0..n-2}
 *            b(i)*b(n-1-i), where B(x) = Sum b(k) x^k is the g.f. of A000598:
 *            B(x) = 1 + x*(B(x)^3 + 3*B(x)*B(x^2) + 2*B(x^3))/6, b(0) = 1.
 *            Computed as b(m+1) = M3(m), where M2 and M3 count multisets of 2
 *            and 3 trees: M2(s) = Sum_{i<s-i} b(i)b(s-i) + [s ev...
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 26, u64 n <= 48, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A632_MAX 20002

static seq_t a632_b[A632_MAX];
static seq_t a632_m2[A632_MAX];
static long long a632_done = -1;
static int a632_mode = -1;   /* 1 = fixed width, 0 = arbitrary precision */
static seq_t a632_inv3;

static void a632_init(void) {
    seq_t x = 1;
    int i;
    for (i = 0; i < 200; i++) x <<= 1;
    a632_mode = (x == 0) ? 1 : 0;
    if (a632_mode) {
        seq_t p = 0;
        for (i = 0; i < 100; i++) p = (seq_t)((p << 2) | 2);
        a632_inv3 = (seq_t)(p + 1);   /* 0xAA...AB = inverse of 3 mod 2^w */
    }
}

static void a632_extend(long long upto) {
    if (a632_done < 0) {
        a632_b[0] = 1;
        a632_done = 0;
    }
    while (a632_done < upto) {
        long long m = a632_done;
        long long i;
        seq_t s = 0;
        seq_t t = 0;
        for (i = 0; 2 * i < m; i++) s += a632_b[i] * a632_b[m - i];
        if (m % 2 == 0) {
            seq_t h = a632_b[m / 2];
            if (h % 2 == 0) s += (h / 2) * (h + 1);
            else s += h * ((h + 1) / 2);
        }
        a632_m2[m] = s;
        for (i = 0; i <= m; i++) t += a632_b[i] * a632_m2[m - i];
        for (i = 0; 2 * i <= m; i++) t += a632_b[i] * a632_b[m - 2 * i];
        if (m % 3 == 0) t += a632_b[m / 3];
        if (a632_mode) a632_b[m + 1] = (seq_t)(t * a632_inv3);
        else a632_b[m + 1] = t / 3;
        a632_done = m + 1;
    }
}

seq_t a(idx_t n) {
    long long N, i;
    seq_t r = 0;
    if (n < 2) return 0;
    if (a632_mode < 0) a632_init();
    N = (long long)n;
    if (N - 1 >= A632_MAX - 1) return 0;
    a632_extend(N - 1);
    for (i = 0; i <= N - 2; i++) r += a632_b[i] * a632_b[N - 1 - i];
    return r;
}
