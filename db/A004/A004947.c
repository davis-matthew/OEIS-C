/*
 * A004947  a(n) = round(n*phi^12), where phi is the golden ratio, A001622.
 * https://oeis.org/A004947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n*phi^12) = floor((322*n + 1 +
 *            floor(144*sqrt(5)*n))/2), since phi^12 = 161 + 72*sqrt(5) and
 *            floor(144*sqrt(5)*n) = isqrt(103680*n^2)
 * Range:     u8 n <= 0, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 33 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

static seq_t u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

static seq_t calc_seq(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t D = N * N * 103680;
    seq_t y = N * (seq_t)3219937887ULL / (seq_t)10000000ULL;
    while ((y + 1) * (y + 1) <= D) y += 1;
    while (y > 0 && y * y > D) y -= 1;
    return (N * 322 + 1 + y) / 2;
}

static unsigned __int128 calc_u128(idx_t n) {
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 D = N * N * 103680;
    unsigned __int128 y = N * (unsigned __int128)3219937887ULL / (unsigned __int128)10000000ULL;
    while ((y + 1) * (y + 1) <= D) y += 1;
    while (y > 0 && y * y > D) y -= 1;
    return (N * 322 + 1 + y) / 2;
}

seq_t a(idx_t n) {
    if (seq_is_big()) return calc_seq(n);
    return u128_to_seq(calc_u128(n));
}
