/*
 * A002710  Triangulations of the disk G_{n,1}.
 * https://oeis.org/A002710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (T(n) + R2(n) + 2*R4(n))/4 (Burnside over the 4 rotations
 *            of the square), where T(n) = 40*(4n+3)!/(n!*(3n+5)!) = rooted
 *            count (Brown), R2(2k) = 4*C(4k+1,k)/(3k+2), R2(2k+1) =
 *            4*C(4k+3,k)/(3k+4) (half-turn symmetric), R4(4j+1) =
 *            2*C(4j+1,j)/(3j+2), R4(n) = 0 otherwise (quarter-turn symmetric)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 43,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 490 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* exact: c * C(N,k) / d1 / d2 in seq_t (arbitrary precision) */
static seq_t a2710_bterm(idx_t N, idx_t k, unsigned c, idx_t d1, idx_t d2) {
    seq_t b = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        b = b * (seq_t)(N - k + i);
        b = b / (seq_t)i;
    }
    b = b * (seq_t)c;
    b = b / (seq_t)d1;
    b = b / (seq_t)d2;
    return b;
}

static void a2710_div(uint64_t *x, uint64_t d) {
    unsigned __int128 r = 0;
    int i;
    for (i = 2; i >= 0; i--) {
        r = (r << 64) | x[i];
        x[i] = (uint64_t)(r / d);
        r %= d;
    }
}

/* 192-bit helpers for fixed-width builds */
static void a2710_mul(uint64_t *x, uint64_t m) {
    unsigned __int128 c = 0;
    int i;
    for (i = 0; i < 3; i++) {
        c += (unsigned __int128)x[i] * m;
        x[i] = (uint64_t)c;
        c >>= 64;
    }
}

static void a2710_fterm(uint64_t *acc, idx_t N, idx_t k, uint64_t c, uint64_t d1, uint64_t d2) {
    uint64_t x[3];
    unsigned __int128 s = 0;
    idx_t i;
    int j;
    x[0] = 1; x[1] = 0; x[2] = 0;
    for (i = 1; i <= k; i++) {
        a2710_mul(x, (uint64_t)(N - k + i));
        a2710_div(x, (uint64_t)i);
    }
    a2710_mul(x, c);
    a2710_div(x, d1);
    a2710_div(x, d2);
    for (j = 0; j < 3; j++) {
        s += (unsigned __int128)acc[j] + x[j];
        acc[j] = (uint64_t)s;
        s >>= 64;
    }
}

static int a2710_isbig(void) {
    seq_t x = 1;
    int i;
    for (i = 0; i < 140; i++) x = x + x;
    return x != 0;
}

seq_t a(idx_t n) {
    idx_t k = n / 2;
    if (a2710_isbig()) {
        seq_t t = a2710_bterm(4 * n + 3, n, 40, 3 * n + 5, 3 * n + 4);
        seq_t r2, r4 = 0;
        if (n % 2 == 0) r2 = a2710_bterm(2 * n + 1, k, 4, 3 * k + 2, 1);
        else r2 = a2710_bterm(2 * n + 1, k, 4, 3 * k + 4, 1);
        if (n % 4 == 1) {
            idx_t j = n / 4;
            r4 = a2710_bterm(4 * j + 1, j, 4, 3 * j + 2, 1);
        }
        return (t + r2 + r4) / 4;
    } else {
        uint64_t acc[3];
        seq_t r;
        int i;
        acc[0] = 0; acc[1] = 0; acc[2] = 0;
        a2710_fterm(acc, 4 * n + 3, n, 40, 3 * n + 5, 3 * n + 4);
        if (n % 2 == 0) a2710_fterm(acc, 2 * n + 1, k, 4, 3 * k + 2, 1);
        else a2710_fterm(acc, 2 * n + 1, k, 4, 3 * k + 4, 1);
        if (n % 4 == 1) {
            idx_t j = n / 4;
            a2710_fterm(acc, 4 * j + 1, j, 4, 3 * j + 2, 1);
        }
        a2710_div(acc, 4);
        r = (seq_t)acc[1];
        for (i = 0; i < 8; i++) r = (seq_t)(r << 8);
        r |= (seq_t)acc[0];
        return r;
    }
}
