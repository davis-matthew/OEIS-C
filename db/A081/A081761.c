/*
 * A081761  Least number >n having same type of prime factorization, a(1)=1.
 * https://oeis.org/A081761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   eq_exps, factor_u64, sort_exps_desc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A081761 – least number > n having the same type of prime factorisation
    a(n) = minimal m > n such that the multiset of prime exponents of m
           equals that of n (a(1)=1, offset 1)                              */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *count) */
#include "../utils/eq_exps.h"
#include "../utils/sort_exps_desc.h"

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;                  /* special case */

    /* factorise n ----------------------------------------------------- */
    uint64_t n64 = (uint64_t)n;
    uint64_t pr[64], exp[64];
    size_t    cnt = 0;
    factor_u64(n64, pr, exp, &cnt);

    /* sort exponents of n ------------------------------------------------ */
    sort_exps_desc(exp, cnt);

    /* search for next integer with identical exponent multiset ---------- */
    uint64_t m = n64 + 1;
    for (;;) {
        uint64_t pr2[64], exp2[64];
        size_t cnt2 = 0;
        factor_u64(m, pr2, exp2, &cnt2);
        sort_exps_desc(exp2, cnt2);
        if (eq_exps(exp2, cnt2, exp, cnt))
            return (seq_t)m;
        ++m;
    }
}
