/*
 * A062358  a(n) = n! / d(n), where d(n) is the number of divisors of n.
 * https://oeis.org/A062358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! / d(n) (d(n) = number of divisors of n)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   build_primes_upto, divisor_count_u64, factor_u64, pow_uint,
 *            seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#include "../utils/divisor_count_u64.h"
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/pow_uint.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Cache primes up to the largest request */
    static uint64_t *prime_list = NULL;
    static size_t   prime_cnt   = 0;
    static idx_t    prime_max   = 0;

    if (n > prime_max) {
        size_t new_cnt;
        build_primes_upto(n, &prime_list, &new_cnt);
        prime_cnt = new_cnt;
        prime_max = n;
    }

    /* Number of divisors of n */
    uint64_t dn = divisor_count_u64((uint64_t)n);

    /* Factorise dn */
    uint64_t pr_f[64];
    uint64_t ex_f[64];
    size_t   cnt_f = 0;
    factor_u64(dn, pr_f, ex_f, &cnt_f);

    seq_t result = (seq_t)1;

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = prime_list[pi];
        if (p > (uint64_t)n) break;

        /* exponent of p in n! (Legendre) */
        idx_t exp_fact = 0;
        idx_t pp = (idx_t)p;
        while (pp <= n) {
            exp_fact += n / pp;
            pp *= p;
        }

        /* exponent of p in d(n) */
        idx_t exp_d = 0;
        for (size_t fi = 0; fi < cnt_f; ++fi) {
            if (pr_f[fi] == p) {
                exp_d = (idx_t)ex_f[fi];
                break;
            }
        }

        idx_t exp = exp_fact - exp_d;
        if (exp == 0) continue;

        seq_t powp = pow_uint(p, exp);
        result = seq_mul(result, powp);
    }

    return result;
}
