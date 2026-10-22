/*
 * A000845  Number of switching networks (see Harrison reference for precise
 *          definition).
 * https://oeis.org/A000845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside's lemma over B_n x Z_2^3, where B_n is the
 *            hyperoctahedral group of permutations and complementations of
 *            the n variables, and Z_2^3 complements the 3 output bits: a(n) =
 *            (1/(8*2^n*n!)) * Sum_{sigma in B_n} 8^{c(sigma)} * (1 + 7*[sigma
 *            has a negative cycle]), where c(sigma) is the number...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   g845
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/g845.h"

static int K845[128], S845[128];
static int np845;
static int big845;
static idx_t n845;
static seq_t F845, tot845;
static unsigned __int128 F128, tot128;

static void leaf845(void)
{
    int i;
    uint64_t N = 1;
    bool neg = false;
    for (i = 0; i < np845; i++) {
        uint64_t L = (uint64_t)K845[i] * (S845[i] ? 2 : 1);
        N = N / g845(N, L) * L;
        if (S845[i]) neg = true;
    }
    unsigned __int128 sum = 0;
    uint64_t j;
    for (j = 0; j < N; j++) {
        uint64_t e = 0;
        bool ok = true;
        for (i = 0; i < np845; i++) {
            uint64_t k = (uint64_t)K845[i];
            uint64_t g = j == 0 ? k : g845(k, j);
            if (S845[i] && ((j / g) & 1)) { ok = false; break; }
            e += g;
        }
        if (ok) sum += ((unsigned __int128)1) << e;
    }
    uint64_t cyc = (uint64_t)(sum / N);
    idx_t sh = (idx_t)(3 * cyc + (neg ? 3 : 0));

    if (big845) {
        seq_t z = 1;
        i = 0;
        while (i < np845) {
            int m = 1, t;
            while (i + m < np845 && K845[i + m] == K845[i] && S845[i + m] == S845[i]) m++;
            for (t = 1; t <= m; t++) {
                z *= (seq_t)(2 * K845[i]);
                z *= (seq_t)t;
            }
            i += m;
        }
        seq_t term = F845 / z;
        term <<= sh;
        tot845 += term;
    } else {
        unsigned __int128 z = 1;
        i = 0;
        while (i < np845) {
            int m = 1, t;
            while (i + m < np845 && K845[i + m] == K845[i] && S845[i + m] == S845[i]) m++;
            for (t = 1; t <= m; t++) {
                z *= (unsigned __int128)(2 * K845[i]);
                z *= (unsigned __int128)t;
            }
            i += m;
        }
        unsigned __int128 term = F128 / z;
        term <<= sh;
        tot128 += term;
    }
}

static void rec845(int rem, int maxk, int maxs)
{
    int k, s;
    if (rem == 0) { leaf845(); return; }
    for (k = rem < maxk ? rem : maxk; k >= 1; k--) {
        int top = (k == maxk) ? maxs : 1;
        for (s = top; s >= 0; s--) {
            K845[np845] = k;
            S845[np845] = s;
            np845++;
            rec845(rem - k, k, s);
            np845--;
        }
    }
}

seq_t a(idx_t n)
{
    int i;
    seq_t x = 1;
    for (i = 0; i < 200; i++) x *= 2;
    big845 = (x != 0);
    n845 = n;
    np845 = 0;
    if (n > 100) return 0;
    if (big845) {
        idx_t t;
        F845 = 1;
        for (t = 1; t <= n; t++) { F845 *= (seq_t)t; F845 *= 2; }
        tot845 = 0;
        rec845((int)n, (int)n, 1);
        return tot845 / (F845 * 8);
    } else {
        idx_t t;
        unsigned __int128 v;
        if (n > 5) return 0; /* a(6) exceeds 128 bits */
        F128 = 1;
        for (t = 1; t <= n; t++) { F128 *= (unsigned __int128)t; F128 *= 2; }
        tot128 = 0;
        rec845((int)n, (int)n, 1);
        v = tot128 / (F128 * 8);
        if ((uint64_t)(v >> 64) == 0 || sizeof(seq_t) < 16)
            return (seq_t)(uint64_t)v;
        {
            seq_t r = (seq_t)(uint64_t)(v >> 64);
            r <<= 32;
            r <<= 32;
            r += (seq_t)(uint64_t)v;
            return r;
        }
    }
}
