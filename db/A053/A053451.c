/*
 * A053451  Multiplicative order of 8 mod 2n+1.
 * https://oeis.org/A053451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = multiplicative order of 8 modulo (2n+1)
 * Range:     u8 n <= 132, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* modulus m = 2n + 1 */
    seq_t m = (seq_t)n * 2 + 1;

    /* Handle the trivial case m == 1 (n = 0) */
    if (m == 1)
        return 1;

    /* Compute φ(m) */
    uint64_t temp_m = (uint64_t)m;
    uint64_t phi_m = temp_m;
    for (uint64_t p = 2; p * p <= temp_m; ++p) {
        if (temp_m % p == 0) {
            while (temp_m % p == 0) temp_m /= p;
            phi_m -= phi_m / p;
        }
    }
    if (temp_m > 1) {
        phi_m -= phi_m / temp_m;
    }

    /* Factor φ(m) into distinct prime factors */
    uint64_t phi_tmp = phi_m;
    uint64_t factors[16];
    int fac_cnt = 0;
    for (uint64_t p = 2; p * p <= phi_tmp; ++p) {
        if (phi_tmp % p == 0) {
            factors[fac_cnt++] = p;
            while (phi_tmp % p == 0) phi_tmp /= p;
        }
    }
    if (phi_tmp > 1)
        factors[fac_cnt++] = phi_tmp;

    /* Compute multiplicative order of 8 modulo m */
    seq_t order = (seq_t)phi_m;
    for (int i = 0; i < fac_cnt; ++i) {
        uint64_t p = factors[i];
        while (order % p == 0) {
            seq_t candidate = mod_pow((seq_t)8, (uint64_t)(order / p), m);
            if (candidate == 1)
                order /= p;
            else
                break;
        }
    }

    return order;
}
