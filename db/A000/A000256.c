/*
 * A000256  Number of simple triangulations of the plane with n nodes.
 * https://oeis.org/A000256
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (3*(3k+1)*(3k+2)/2 * a(n-2) + (52k^2-7k-15) * a(n-1)) /
 *            (4*(k+1)*(2k+3)), k = n-4, for n >= 6; a(3) = 1, a(4) = 1, a(5)
 *            = 0. (This is the OEIS P-recurrence (1/4+7/8k-9/8k^3)a(k+2) +
 *            (-5/4+2/3k+59/12k^2-13/3k^3)a(k+3) + (-1-2/3k+k^2+2/3k^3)a(k+4)
 *            = 0 with the common factor 2(k-1) cancelled.)
 * Range:     u8 n <= 10, u16 n <= 13, u32 n <= 19, u64 n <= 32, u128 n <= 56,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 a256_u128;

static seq_t *a256_memo = 0;
static idx_t a256_cap = 0, a256_top = 0;
static a256_u128 a256_w2, a256_w1;
static int a256_wide = 0;

static void a256_grow(idx_t need)
{
    idx_t nc = a256_cap ? a256_cap : 64;
    while (nc <= need) nc *= 2;
    a256_memo = (seq_t *)realloc(a256_memo, (size_t)nc * sizeof(seq_t));
    memset((void *)(a256_memo + a256_cap), 0, (size_t)(nc - a256_cap) * sizeof(seq_t));
    a256_cap = nc;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    if (!a256_memo) {
        a256_grow(16);
        a256_memo[3] = 1;
        a256_memo[4] = 1;
        a256_memo[5] = 0;
        a256_top = 5;
        a256_w2 = 1;  /* a(4) */
        a256_w1 = 0;  /* a(5) */
        a256_wide = 0;
    }
    if (n >= a256_cap) a256_grow(n);
    while (a256_top < n) {
        idx_t m = a256_top + 1;
        idx_t k = m - 4;
        idx_t A = 3 * (3 * k + 1) * (3 * k + 2) / 2;
        idx_t B = 52 * k * k - 7 * k - 15;
        idx_t D = 4 * (k + 1) * (2 * k + 3);
        if (!a256_wide) {
            a256_u128 x = a256_w2, y = a256_w1, d = (a256_u128)D;
            a256_u128 v = (a256_u128)A * (x / d) + (a256_u128)B * (y / d)
                        + ((a256_u128)A * (x % d) + (a256_u128)B * (y % d)) / d;
            a256_memo[m] = u128_to_seq(v);
            a256_w2 = a256_w1;
            a256_w1 = v;
            if ((v >> 100) != 0) a256_wide = 1;
        } else {
            seq_t x = a256_memo[m - 2], y = a256_memo[m - 1];
            seq_t Ds = (seq_t)D;
            seq_t qx = x / Ds, qy = y / Ds;
            unsigned long long rx = (unsigned long long)(x % Ds);
            unsigned long long ry = (unsigned long long)(y % Ds);
            a256_u128 r = ((a256_u128)A * rx + (a256_u128)B * ry) / (a256_u128)D;
            seq_t v = qx * (seq_t)A;
            v += qy * (seq_t)B;
            v += u128_to_seq(r);
            a256_memo[m] = v;
        }
        a256_top = m;
    }
    return a256_memo[n];
}
