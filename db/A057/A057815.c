/*
 * A057815  a(n) = gcd(n,binomial(n,floor(n/2))).
 * https://oeis.org/A057815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( n, binomial(n, floor(n/2)) )
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*        A057815: a(2k+1)=2k+1, a(2k)=gcd(2k, binom(2k,k)) */

#include <stdint.h>

/* Compute the sequence value */
seq_t a(idx_t n)
{
    /* Odd indices: answer is n itself */
    if ((n & 1u) != 0)
        return (seq_t)n;

    /* Even indices: let k = n/2 and compute gcd(n, binom(n,k)) */
    uint64_t n_val = (uint64_t)n;          /* safe cast – n fits in 64 bits */
    uint64_t k = n_val >> 1;               /* n/2 */
    seq_t result = 1;

    uint64_t m = n_val;

    /* Factorise n_val by trial division */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p != 0)
            continue;

        /* exponent of p in n */
        unsigned exp_n = 0;
        while (m % p == 0) {
            m /= p;
            ++exp_n;
        }

        /* exponent of p in binomial(n,k) : v_p(n!) - 2*v_p(k!) */
        unsigned v_binom = 0;
        uint64_t pow = p;
        while (pow <= n_val) {
            v_binom += (unsigned)(n_val / pow);
            if (pow > n_val / p) break;
            pow *= p;
        }
        pow = p;
        while (pow <= k) {
            v_binom -= 2 * (unsigned)(k / pow);
            if (pow > k / p) break;
            pow *= p;
        }

        unsigned min_exp = exp_n < v_binom ? exp_n : v_binom;
        for (unsigned i = 0; i < min_exp; ++i)
            result *= (seq_t)p;
    }

    /* Remaining prime factor, if any */
    if (m > 1) {
        uint64_t p = m;
        unsigned exp_n = 1;

        unsigned v_binom = 0;
        uint64_t pow = p;
        while (pow <= n_val) {
            v_binom += (unsigned)(n_val / pow);
            if (pow > n_val / p) break;
            pow *= p;
        }
        pow = p;
        while (pow <= k) {
            v_binom -= 2 * (unsigned)(k / pow);
            if (pow > k / p) break;
            pow *= p;
        }

        unsigned min_exp = exp_n < v_binom ? exp_n : v_binom;
        for (unsigned i = 0; i < min_exp; ++i)
            result *= (seq_t)p;
    }

    return result;
}
