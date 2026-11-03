/*
 * A002543  Complete Post functions of n variables.
 * https://oeis.org/A002543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (1/n!) * Sum_{s in S_n} ( 2^(c(s)-2) - [s has a cycle of
 *            odd length] * 2^(c(s)/2-1) ), where c(s) = number of cycles of s
 *            acting on {0,1}^n = (1/L) * Sum_{d|L} phi(L/d) * 2^(Sum_i
 *            gcd(l_i,d)), with l_i the cycle lengths of s and L = lcm(l_i).
 *            (Burnside count of Sheffer (complete) Boolean f...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-01)
 * Helpers:   gcd_u64, pow2_u64, totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd_u64.h"
#include "../utils/pow2_u64.h"
#include "../utils/totient_u64.h"

static void a2543_leaf(const int *parts, int k, seq_t fact, seq_t *total)
{
    seq_t z = 1;
    uint64_t L = 1, d, c;
    unsigned __int128 sum = 0;
    int i, run = 0, all_even = 1;
    for (i = 0; i < k; i++) {
        z *= (seq_t)parts[i];
        if (i > 0 && parts[i] == parts[i - 1]) run++;
        else run = 1;
        z *= (seq_t)run;
        L = L / gcd_u64(L, (uint64_t)parts[i]) * (uint64_t)parts[i];
        if (parts[i] % 2 != 0) all_even = 0;
    }
    for (d = 1; d <= L; d++) {
        if (L % d == 0) {
            uint64_t s = 0;
            for (i = 0; i < k; i++) s += gcd_u64((uint64_t)parts[i], d);
            sum += (unsigned __int128)totient_u64(L / d) * ((unsigned __int128)1 << s);
        }
    }
    c = (uint64_t)(sum / L);
    {
        seq_t term = pow2_u64(c - 2);
        if (!all_even) term -= pow2_u64(c / 2 - 1);
        *total += (fact / z) * term;
    }
}

static void a2543_rec(int rem, int maxp, int *parts, int k, seq_t fact, seq_t *total)
{
    int p;
    if (rem == 0) {
        a2543_leaf(parts, k, fact, total);
        return;
    }
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        parts[k] = p;
        a2543_rec(rem - p, p, parts, k + 1, fact, total);
    }
}

seq_t a(idx_t n)
{
    seq_t fact = 1, total = 0;
    idx_t i;
    int *parts;
    for (i = 2; i <= n; i++) fact *= (seq_t)i;
    parts = (int *)malloc(sizeof(int) * (size_t)(n + 1));
    a2543_rec((int)n, (int)n, parts, 0, fact, &total);
    free(parts);
    return total / fact;
}
