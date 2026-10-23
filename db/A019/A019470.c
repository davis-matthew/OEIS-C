/*
 * A019470  Numbers k that divide binomial(2*k-4, k-2).
 * https://oeis.org/A019470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 220, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k that divide binomial(2*k-4, k-2) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compute v_p(n!) for given prime p */
static idx_t v_factorial(idx_t n, idx_t p)
{
    idx_t res = 0;
    while (n) {
        n /= p;
        res += n;
    }
    return res;
}

/* Check whether k divides C(2*k-4, k-2) for a single prime factor p^e of k */
static bool good_factor(idx_t k, idx_t p, idx_t e)
{
    /* v_p(binomial(2k-4, k-2)) = v_p((2k-4)!) - 2*v_p((k-2)!) */
    idx_t vp_n1 = v_factorial(2*k-4, p);
    idx_t vp_n2 = v_factorial(k-2,   p);
    idx_t vp_binom = vp_n1 - 2*vp_n2;
    return vp_binom >= e;
}

/* Test if k satisfies the divisibility condition */
static bool _k_divides_binomial(idx_t k)
{
    if (k <= 1) return true;   /* k = 1 is always accepted */

    idx_t tmp = k;
    /* trial division by 2 separately */
    if ((tmp & 1) == 0) {
        idx_t e = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++e;
        }
        if (!good_factor(k, 2, e)) return false;
    }
    /* odd divisors */
    for (idx_t p = 3; p*p <= tmp; p += 2) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            if (!good_factor(k, p, e)) return false;
        }
    }
    /* remaining prime factor, if any */
    if (tmp > 1) {
        if (!good_factor(k, tmp, 1)) return false;
    }
    return true;
}

/* OEIS sequence A019470 */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        if (_k_divides_binomial(k)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
    }
}
