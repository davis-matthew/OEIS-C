/*
 * A000812  Number of switching networks (see Harrison reference for precise
 *          definition).
 * https://oeis.org/A000812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside's lemma for S_n acting on the 2^n input vectors: a(n) =
 *            (1/n!) * Sum_{cycle types L of S_n} (n!/z(L)) * 8^c(L), where
 *            c(L) = (1/m) * Sum_{j=0..m-1} 2^(Sum_i gcd(l_i, j)), m = lcm(L),
 *            is the number of cycles induced on {0,1}^n (gcd(l,0) = l)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   sw_gcd, sw_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sw_gcd.h"
#include "../utils/sw_pow.h"

static int sw_parts[256];
static int sw_np;
static seq_t sw_total;
static seq_t sw_fact;

static void sw_process(void) {
    uint64_t m = 1;
    int i;
    for (i = 0; i < sw_np; i++) {
        uint64_t l = (uint64_t)sw_parts[i];
        m = m / sw_gcd(m, l) * l;
    }
    unsigned __int128 s = 0;
    uint64_t j;
    for (j = 0; j < m; j++) {
        int e = 0;
        for (i = 0; i < sw_np; i++)
            e += (int)sw_gcd((uint64_t)sw_parts[i], j);
        s += ((unsigned __int128)1) << e;
    }
    idx_t c = (idx_t)(s / m);

    /* z = prod l^{k_l} * k_l! */
    seq_t z = 1;
    i = 0;
    while (i < sw_np) {
        int l = sw_parts[i];
        int k = 0;
        while (i < sw_np && sw_parts[i] == l) {
            k++;
            z *= (seq_t)l;
            z *= (seq_t)k;
            i++;
        }
    }
    sw_total += (sw_fact / z) * sw_pow((seq_t)8, c);
}

static void sw_rec(int rem, int maxp) {
    if (rem == 0) { sw_process(); return; }
    int p;
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        sw_parts[sw_np++] = p;
        sw_rec(rem - p, p);
        sw_np--;
    }
}

seq_t a(idx_t n) {
    int nn = (int)n;
    idx_t i;
    sw_fact = 1;
    for (i = 2; i <= n; i++) sw_fact *= (seq_t)i;
    sw_total = 0;
    sw_np = 0;
    sw_rec(nn, nn);
    return sw_total / sw_fact;
}
