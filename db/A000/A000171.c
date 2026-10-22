/*
 * A000171  Number of self-complementary graphs with n nodes.
 * https://oeis.org/A000171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (1/n!) * Sum_{s in S_n} Prod_{edge cycles of s} [cycle
 *            length even]*2 (Read's count of self-complementary graphs). Only
 *            permutations whose cycles all have length divisible by 4, plus
 *            at most one fixed point (n = 4m or 4m+1), contribute. Writing
 *            the cycle lengths as 4*j_i, (j_i) a partition...
 * Range:     u8 n <= 11, u16 n <= 15, u32 n <= 19, u64 n <= 27, u128 n <= 36,
 *            big any n
 * Helpers:   sc_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sc_gcd.h"

static seq_t sc_Q, sc_R, sc_MF;
static long long *sc_parts;
static long long sc_m;
static int sc_odd;

static void sc_rec(long long rem, long long maxp, long long np, long long gsum,
                   long long lastmult, seq_t z)
{
    if (rem == 0) {
        long long e = 2 * sc_m - 2 * np + 4 * gsum + (sc_odd ? np : 0);
        seq_t q = 0, r = 1;
        long long i;
        if (r >= z) { q = 1; r = 0; }
        for (i = 0; i < e; i++) {
            q += q;
            r += r;
            if (r >= z) { r -= z; q += 1; }
        }
        sc_Q += q;
        if (r != 0) sc_R += r * (sc_MF / z);
        return;
    }
    {
        long long j = rem < maxp ? rem : maxp;
        for (; j >= 1; j--) {
            long long g = 0, i, mult;
            for (i = 0; i < np; i++) g += sc_gcd(j, sc_parts[i]);
            mult = (np > 0 && sc_parts[np - 1] == j) ? lastmult + 1 : 1;
            sc_parts[np] = j;
            sc_rec(rem - j, j, np + 1, gsum + g, mult, z * (seq_t)j * (seq_t)mult);
        }
    }
}

seq_t a(idx_t n)
{
    long long i;
    seq_t res;
    if (n % 4 == 2 || n % 4 == 3) return 0;
    sc_m = (long long)(n / 4);
    sc_odd = (n % 4 == 1);
    sc_MF = 1;
    for (i = 2; i <= sc_m; i++) sc_MF *= (seq_t)i;
    sc_Q = 0;
    sc_R = 0;
    sc_parts = (long long *)malloc(sizeof(long long) * (size_t)(sc_m + 1));
    sc_rec(sc_m, sc_m, 0, 0, 0, (seq_t)1);
    free(sc_parts);
    res = sc_Q + sc_R / sc_MF;
    return res;
}
