/*
 * A001727  Weight distribution of [64,42,8] 3rd-order Reed-Muller code of
 *          length 64.
 * https://oeis.org/A001727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = A_{2n}, the number of codewords of weight 2n in RM(3,6).
 *            RM(3,6) is the dual of RM(2,6), so A_w = 2^(-22) * Sum_i B_i *
 *            K_w(i), where K_w(i) = Sum_s (-1)^s C(i,s) C(64-i,w-s) is the
 *            Krawtchouk polynomial (MacWilliams identity). B_i is the weight
 *            distribution of RM(2,6) (Sloane-Berlekamp):...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static __int128 a001727_res[33];
static int a001727_done = 0;

static void a001727_init(void) {
    static long long C[65][65];
    __int128 B[65];
    __int128 sum = 0;
    int i, j, s, h;
    const int m = 6;
    for (i = 0; i <= 64; i++) {
        C[i][0] = 1;
        for (j = 1; j <= 64; j++)
            C[i][j] = (i == 0) ? 0 : C[i - 1][j - 1] + C[i - 1][j];
    }
    for (i = 0; i <= 64; i++) B[i] = 0;
    B[0] = 1;
    B[64] = 1;
    for (h = 1; h <= m / 2; h++) {
        __int128 num = 1, den = 1, A;
        for (i = 0; i < 2 * h; i++) num *= (((__int128)1 << (m - i)) - 1);
        for (i = 1; i <= h; i++) den *= (((__int128)1 << (2 * i)) - 1);
        A = ((__int128)1 << (h * (h + 1))) * (num / den);
        B[32 - (1 << (m - 1 - h))] += A;
        B[32 + (1 << (m - 1 - h))] += A;
        sum += 2 * A;
    }
    B[32] = ((__int128)1 << 22) - 2 - sum;
    for (j = 0; j <= 32; j++) {
        int w = 2 * j;
        __int128 tot = 0;
        for (i = 0; i <= 64; i++) {
            __int128 k = 0;
            if (B[i] == 0) continue;
            for (s = 0; s <= w; s++) {
                if (s <= i && w - s <= 64 - i) {
                    __int128 t = (__int128)C[i][s] * (__int128)C[64 - i][w - s];
                    if (s & 1) k -= t; else k += t;
                }
            }
            tot += B[i] * k;
        }
        a001727_res[j] = tot / ((__int128)1 << 22);
    }
    a001727_done = 1;
}

seq_t a(idx_t n) {
    if (!a001727_done) a001727_init();
    if (n > 32) return 0;
    return (seq_t)(uint64_t)a001727_res[n];
}
