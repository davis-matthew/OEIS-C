/*
 * A007792  a(n) = (1/n)*Sum_{x in (Z/nZ)^6} e^(2*Pi*i*det(circ(x))/n), the
 *          normalized Gauss sum of the 6 X 6 circulant determinant.
 * https://oeis.org/A007792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, changed
 * Formula:   a(n) is multiplicative. For p == 1 (mod 6): a(p^k) = p^(5*k) -
 *            (p-1)^5 * p^(4*(k-1)) * Sum_{j=0..k-1}
 *            binomial(j+4,4)*p^(k-1-j); for p == 5 (mod 6): a(p^k) = p^(5*k)
 *            - (p-1)*(p^2-1)^2 * p^(4*(k-1)) * Sum_{j=0..k-1}
 *            binomial(floor(j/2)+2,2)*p^(k-1-j). For p = 2, 3: p^k*a(p^k) =
 *            #{x mod p^k : p^k |...
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 132, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a000272_ipow, a000707_binom, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a000272_ipow.h"
#include "../utils/a000707_binom.h"
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128_t;

static u128_t upow128(u128_t b, unsigned e) {
    u128_t r = 1;
    while (e--) r *= b;
    return r;
}

/* number of residues mod q^K with valuation i (i == K means zero), base q, unit factor (q-1) */
static u128_t cnt_val(unsigned q, unsigned K, unsigned i) {
    if (i >= K) return 1;
    return (u128_t)(q - 1) * upow128(q, K - i - 1);
}

static seq_t a_ramified(unsigned p, unsigned k) {
    u128_t D1[64], D2[64];
    unsigned i, l;
    for (i = 0; i <= k; i++) { D1[i] = 0; D2[i] = 0; }
    if (p == 2) {
        /* a^2-b^2 = u*w, u == w (mod 2), each (u,w) hit twice */
        D1[0] = upow128(2, 2 * k - 1);
        for (i = 1; i <= k; i++)
            for (l = 1; l <= k; l++) {
                unsigned s = i + l; if (s > k) s = k;
                D1[s] += 2 * cnt_val(2, k, i) * cnt_val(2, k, l);
            }
        /* N_R(U*W) over R = Z_2[w], U == W (mod 2), each hit 4 times, v = 2*(vU+vW) */
        D2[0] = (u128_t)12 * upow128(4, 2 * k - 2);
        for (i = 1; i <= k; i++)
            for (l = 1; l <= k; l++) {
                unsigned s = 2 * (i + l); if (s > k) s = k;
                D2[s] += 4 * cnt_val(4, k, i) * cnt_val(4, k, l);
            }
    } else {
        /* 3x3 circulant det = s * N(z), z in Z_3[zeta3], s == z (mod pi), each hit 3 times */
        D1[0] = (u128_t)6 * upow128(3, k - 1) * upow128(3, 2 * k - 1);
        for (i = 1; i <= k; i++)
            for (l = 1; l <= 2 * k; l++) {
                unsigned s = i + l; if (s > k) s = k;
                D1[s] += 3 * cnt_val(3, k, i) * cnt_val(3, 2 * k, l);
            }
        for (i = 0; i <= k; i++) D2[i] = D1[i];
    }
    u128_t ge = 0, eq = 0;
    for (i = 0; i <= k; i++)
        for (l = 0; l <= k; l++) {
            if (i + l >= k) ge += D1[i] * D2[l];
            else if (i + l == k - 1) eq += D1[i] * D2[l];
        }
    u128_t num = ge * (u128_t)(p - 1) - eq;
    num /= (u128_t)(p - 1);
    num /= upow128(p, k);
    return u128_to_seq(num);
}

static seq_t a_prime_power(uint64_t p, idx_t k) {
    if (p == 2 || p == 3) return a_ramified((unsigned)p, (unsigned)k);

    seq_t p_seq = (seq_t)p;
    seq_t p5k = a000272_ipow(p_seq, (idx_t)(5 * k));
    seq_t p4km4 = (k == 1) ? (seq_t)1 : a000272_ipow(p_seq, (idx_t)(4 * (k - 1)));
    seq_t sum = 0;
    idx_t j;

    if (p % 6 == 1) {
        seq_t pm1_5 = a000272_ipow(p_seq - 1, 5);
        for (j = 0; j < k; j++)
            sum += a000707_binom(j + 4, 4) * a000272_ipow(p_seq, (idx_t)(k - 1 - j));
        return p5k - pm1_5 * p4km4 * sum;
    } else {
        seq_t pm1 = p_seq - 1;
        seq_t p2m1 = p_seq * p_seq - 1;
        for (j = 0; j < k; j++)
            sum += a000707_binom(j / 2 + 2, 2) * a000272_ipow(p_seq, (idx_t)(k - 1 - j));
        return p5k - pm1 * p2m1 * p2m1 * p4km4 * sum;
    }
}

seq_t a(idx_t n) {
    seq_t result = (seq_t)1;
    uint64_t temp = n;
    uint64_t p;
    if (n <= 1) return result;
    for (p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            idx_t k = 0;
            while (temp % p == 0) {
                k++;
                temp /= p;
            }
            result *= a_prime_power(p, k);
        }
    }
    if (temp > 1) result *= a_prime_power(temp, 1);
    return result;
}
