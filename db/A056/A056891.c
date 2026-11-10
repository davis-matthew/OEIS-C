/*
 * A056891  a(n) = (s(n)-(n mod 2)) / n where s(n) is A006533.
 * https://oeis.org/A056891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (s(n) - (n mod 2)) / n, where s(n) = A006533(n) (regions
 *            of a circle cut by all chords of n equally spaced points,
 *            Poonen-Rubinstein formula)
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 941, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 8672 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A006533_a(idx_t n) {
    if (n < 3) return (seq_t)n;
    seq_t sn = (seq_t)n;
    seq_t c2 = sn * (sn - 1) / 2;
    seq_t c3 = c2 * (sn - 2) / 3;
    seq_t c4 = c3 * (sn - 3) / 4;
    seq_t base = c4 + c2 + 1;

    __int128 m = (__int128)n;
    __int128 x = 0; /* 48 * correction */
    if (n % 2 == 0) x += -5 * m * m * m + 42 * m * m - 40 * m - 48;
    if (n % 4 == 0) x -= 36 * m;
    if (n % 6 == 0) x += 4 * (-53 * m * m + 310 * m);
    if (n % 12 == 0) x += 1176 * m;
    if (n % 18 == 0) x += 1536 * m;
    if (n % 24 == 0) x += 912 * m;
    if (n % 30 == 0) x -= 1728 * m;
    if (n % 42 == 0) x -= 2400 * m;
    if (n % 60 == 0) x -= 9120 * m;
    if (n % 84 == 0) x -= 3744 * m;
    if (n % 90 == 0) x -= 2304 * m;
    if (n % 120 == 0) x -= 3744 * m;
    if (n % 210 == 0) x -= 2304 * m;

    __int128 c = x / 48;
    if (c >= 0) {
        unsigned __int128 u = (unsigned __int128)c;
        seq_t hi = (seq_t)(unsigned long long)(u >> 64);
        seq_t lo = (seq_t)(unsigned long long)u;
        seq_t cv = hi;
        cv <<= 32; cv <<= 32;
        cv += lo;
        return base + cv;
    } else {
        unsigned __int128 u = (unsigned __int128)(-c);
        seq_t hi = (seq_t)(unsigned long long)(u >> 64);
        seq_t lo = (seq_t)(unsigned long long)u;
        seq_t cv = hi;
        cv <<= 32; cv <<= 32;
        cv += lo;
        return base - cv;
    }
}

seq_t a(idx_t n) {
    seq_t s = A006533_a(n);
    if (n % 2 == 1) s -= 1;
    return s / (seq_t)n;
}
