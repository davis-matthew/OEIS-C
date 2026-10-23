/*
 * A036391  a(n) = sum of order of a mod n, 0 < a < n, gcd(a, n) = 1.
 * https://oeis.org/A036391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = sum_{1 <= a < n, gcd(a,n)=1} ord_n(a)
 * Range:     u8 n <= 22, u16 n <= 316, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   calc_phi, factor_u64, gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of the multiplicative orders of all elements of the
   reduced residue system modulo n.  Uses simple factorisation of
   φ(n) and the standard order‑finding by prime‑power reduction. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"
#include "../utils/calc_phi.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* Handle trivial case n<=1 */
    if (n <= 1) return (seq_t)0;

    /* φ(n) */
    seq_t phi_seq = calc_phi((seq_t)n);
    idx_t phi = (idx_t)phi_seq;                 /* φ(n) fits in 64‑bit for n<=10000 */

    /* Factorise φ(n) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)phi, primes, exps, &pf_cnt);

    seq_t sum = (seq_t)0;
    for (idx_t a_val = 1; a_val < n; ++a_val) {
        /* Skip non‑units */
        if (gcd((seq_t)a_val, (seq_t)n) != 1) continue;

        idx_t order = phi;
        /* Reduce order by prime powers of φ(n) */
        for (size_t i = 0; i < pf_cnt; ++i) {
            uint64_t p = primes[i];
            while ((order % p) == 0) {
                idx_t cand = order / p;
                if (mod_pow((seq_t)a_val, cand, (seq_t)n) == 1)
                    order = cand;
                else
                    break;
            }
        }
        sum += (seq_t)order;
    }
    return sum;
}
