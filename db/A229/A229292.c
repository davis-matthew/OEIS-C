/*
 * A229292  Exponent of the group of 2 X 2 invertible matrices over Z/nZ.
 * https://oeis.org/A229292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(p^s) = lcm(p*(p^(2*s) - 1), p - 1); if gcd(m,n)=1 then a(n*m)
 *            = lcm(a(n), a(m))
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;

/* lcm for seq_t */
static seq_t lcm_seq(seq_t a, seq_t b) {
    seq_t g = gcd(a, b);
    seq_t t = a / g;
    return t * b;
}

/* exponent of GL(2, Z/p^s Z) */
static seq_t local_exp(idx_t p, idx_t s) {
    /* compute p^(2*s) */
    seq_t pow_val = 1;
    for (idx_t i = 0; i < 2 * s; ++i)
        pow_val *= (seq_t)p;
    seq_t val1 = (seq_t)p * (pow_val - 1);      /* p*(p^(2*s)-1) */
    seq_t val2 = (seq_t)(p - 1);                 /* p-1 */
    return lcm_seq(val1, val2);
}

seq_t a(idx_t n) {
    if (n <= 1)
        return 1;

    /* initialize prime list once */
    if (!prime_list) {
        /* 10000 is enough for n up to 10⁴; grow if needed */
        build_primes_upto(100000, &prime_list, &prime_cnt);
    }

    seq_t result = 1;
    idx_t rem = n;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > rem) /* avoid overflow: if p*p > rem, break */
            break;

        if (rem % p != 0)
            continue;

        /* count exponent s */
        idx_t s = 0;
        while (rem % p == 0) {
            rem /= p;
            ++s;
        }

        result = lcm_seq(result, local_exp(p, s));
        if (rem == 1)
            break;
    }

    /* leftover prime factor */
    if (rem > 1) {
        result = lcm_seq(result, local_exp(rem, 1));
    }

    return result;
}
