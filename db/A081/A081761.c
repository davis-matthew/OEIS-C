/*
 * A081761  Least number >n having same type of prime factorization, a(1)=1.
 * https://oeis.org/A081761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 8862 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A081761 – least number > n having the same type of prime factorisation
    a(n) = minimal m > n such that the multiset of prime exponents of m
           equals that of n (a(1)=1, offset 1)                              */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <stdlib.h>
#include "../utils/factor_u64.h"

/* compare two exponent multisets (already sorted)                       */
static bool eq_exps(const uint64_t *a, size_t ka, const uint64_t *b, size_t kb)
{
    if (ka != kb) return false;
    for (size_t i = 0; i < ka; ++i)
        if (a[i] != b[i]) return false;
    return true;
}

/* sort an array of exponents in non‑increasing order                     */
static void sort_exps_desc(uint64_t *exps, size_t k)
{
    for (size_t i = 0; i < k; ++i)
        for (size_t j = i + 1; j < k; ++j)
            if (exps[i] < exps[j]) {
                uint64_t t = exps[i];
                exps[i] = exps[j];
                exps[j] = t;
            }
}

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
