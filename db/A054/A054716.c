/*
 * A054716  Number of powers of 16 modulo n.
 * https://oeis.org/A054716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil(v2(n)/4) + ord_{odd_part(n)}(16)
 * Range:     u8 n <= 521, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* compute the exponent of 2 in n */
    idx_t a2 = 0;
    idx_t tmp = n;
    while ((tmp & 1) == 0) {
        ++a2;
        tmp >>= 1;
    }

    /* number of distinct residues contributed by the 2‑power factor */
    idx_t t2 = (a2 == 0) ? 0 : (a2 + 3) / 4;

    /* odd part of n */
    idx_t odd = tmp;          /* n >> a2 */

    /* multiplicative order of 16 modulo odd (if odd > 1) */
    idx_t ord = 1;
    if (odd > 1) {
        /* φ(odd) */
        idx_t phi = odd;
        idx_t tf = odd;
        for (idx_t p = 2; p * p <= tf; ++p) {
            if (tf % p == 0) {
                phi = phi / p * (p - 1);
                while (tf % p == 0) tf /= p;
            }
        }
        if (tf > 1) {
            phi = phi / tf * (tf - 1);
        }

        /* factor φ to obtain its unique prime factors */
        idx_t primes[16];
        size_t pcnt = 0;
        tf = phi;
        for (idx_t p = 2; p * p <= tf; ++p) {
            if (tf % p == 0) {
                primes[pcnt++] = p;
                while (tf % p == 0) tf /= p;
            }
        }
        if (tf > 1) {
            primes[pcnt++] = tf;
        }

        /* compute the order from φ by dividing out prime powers */
        idx_t o = phi;
        for (size_t i = 0; i < pcnt; ++i) {
            idx_t p = primes[i];
            while ((o % p) == 0) {
                seq_t val = mod_pow((seq_t)16, o / p, (seq_t)odd);
                if (val == (seq_t)1) {
                    o /= p;
                } else {
                    break;
                }
            }
        }
        ord = o;
    }

    return (seq_t)(t2 + ord);
}
