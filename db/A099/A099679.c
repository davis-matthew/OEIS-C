/*
 * A099679  Least m such that repunit R_m is a multiple of A045572(n) (i.e.,
 *          odd numbers not divisible by 5).
 * https://oeis.org/A099679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the least m such that the repunit R_m is a * multiple of
 *            the n-th odd integer not divisible by 5. * * The odd integers
 *            not divisible by 5 are 1,3,7,9,11,13,17,19,21,23,27,... * For
 *            each such integer p, a(n) is the multiplicative order of 10
 *            modulo * 9·p (since R_m ≡ 0 (mod p) ⇔ 10^m ≡ 1 (m...
 * Range:     u8 n <= 11, u16 n <= 3235, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9279 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/powmod.h"

/* Compute the n-th odd integer that is not divisible by 5. */
static inline seq_t
odd_non5(idx_t n)
{
    seq_t p = 1;        /* the first term */
    idx_t count = 1;    /* how many terms found so far */

    while (count < n) {
        p += 2;                 /* only odd numbers */
        if (p % 5 != 0)         /* skip odd multiples of 5 */
            ++count;
    }
    return p;
}

/* Return the multiplicative order of 10 modulo mod. */
static inline seq_t
order_of_10(seq_t mod)
{
    seq_t phi = mod;
    seq_t temp = mod;

    /* compute φ(mod) */
    for (idx_t f = 2; (seq_t)f * (seq_t)f <= temp; ++f) {
        if (temp % (seq_t)f == 0) {
            while (temp % (seq_t)f == 0)
                temp /= (seq_t)f;
            phi = (phi / (seq_t)f) * ((seq_t)f - 1);
        }
    }
    if (temp > 1)
        phi = (phi / temp) * (temp - 1);

    /* collect distinct prime factors of φ(mod) */
    seq_t prime_factors[64];
    idx_t pf_cnt = 0;
    temp = phi;
    for (idx_t f = 2; (seq_t)f * (seq_t)f <= temp; ++f) {
        if (temp % (seq_t)f == 0) {
            prime_factors[pf_cnt++] = (seq_t)f;
            while (temp % (seq_t)f == 0)
                temp /= (seq_t)f;
        }
    }
    if (temp > 1)
        prime_factors[pf_cnt++] = temp;

    /* reduce φ(mod) to the order */
    seq_t order = phi;
    for (idx_t i = 0; i < pf_cnt; ++i) {
        seq_t prime = prime_factors[i];
        while (order % prime == 0) {
            seq_t cand = order / prime;
            if (powmod(10, (idx_t)cand, mod) == (seq_t)1)
                order = cand;
            else
                break;
        }
    }
    return order;
}

seq_t a(idx_t n)
{
    seq_t p = odd_non5(n);          /* the n-th odd integer not divisible by 5 */
    seq_t mod = p * 9;              /* modulus 9·p */
    return order_of_10(mod);        /* least m such that R_m ≡ 0 (mod p) */
}
