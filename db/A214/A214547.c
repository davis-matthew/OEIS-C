/*
 * A214547  Deficient numbers for which the (absolute value of) abundance is
 *          not a divisor.
 * https://oeis.org/A214547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth deficient number whose deficiency does not divide
 *            the number. Computation: iterate integers, test deficiency via
 *            σ(n), retain those that meet the condition, and return the n‑th
 *            such number
 * Range:     u8 n <= 181, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"

static seq_t sigma_of_uint64(uint64_t n, idx_t *spf)
{
    if (n == 1) return 1;          /* σ(1) = 1 */
    uint64_t temp = n;
    seq_t sigma = 1;
    while (temp > 1) {
        uint64_t p = spf[temp];
        uint64_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            e++;
        }
        seq_t term = 1;
        seq_t powv  = 1;
        for (uint64_t i = 0; i < e; ++i) {
            powv *= (seq_t)p;
            term += powv;
        }
        sigma *= term;
    }
    return sigma;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* Prepare a sieve for smallest prime factors up to a safe bound. */
    static idx_t *spf = NULL;
    static int spf_ready = 0;
    if (!spf_ready) {
        const idx_t LIMIT = 2000000ULL;   /* large enough for n up to about 10000 */
        build_spf(LIMIT, &spf);
        spf_ready = 1;
    }

    idx_t count = 0;
    seq_t candidate = 3;   /* first positive deficient number that can be in the sequence */

    while (1) {
        seq_t sigma = sigma_of_uint64((uint64_t)candidate, spf);
        seq_t two_c = (seq_t)2 * candidate;

        if (sigma < two_c) {               /* candidate is deficient */
            seq_t deficiency = two_c - sigma;
            if (candidate % deficiency != 0) {
                ++count;
                if (count == n) return candidate;
            }
        }
        ++candidate;
    }
}
