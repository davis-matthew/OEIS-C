/*
 * A191613  Number of even divisors of lambda(n).
 * https://oeis.org/A191613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of even divisors of λ(n) (Carmichael function)
 * Range:     u8 n <= 9866, u16 n <= 9866, u32 n <= 9866, u64 n <= 9866,
 *            u128 n <= 9866, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static void add_exp(uint64_t *pr, uint64_t *ex, size_t *cnt, uint64_t p, uint64_t e)
{
    for (size_t i = 0; i < *cnt; ++i) {
        if (pr[i] == p) {
            if (e > ex[i]) ex[i] = e;
            return;
        }
    }
    /* new entry */
    pr[*cnt] = p;
    ex[*cnt] = e;
    (*cnt)++;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined, but safe */

    /* factor n */
    uint64_t npr[128], nex[128];
    size_t ncnt;
    factor_u64((uint64_t)n, npr, nex, &ncnt);

    /* store the prime‑exponent table of λ(n) */
    uint64_t lpr[256], lex[256];
    size_t lcnt = 0;

    for (size_t i = 0; i < ncnt; ++i) {
        uint64_t p = npr[i];
        uint64_t e = nex[i];

        if (p == 2) {
            /* λ(2^e) = 1 if e==1, 2 if e==2, 2^{e-2} if e>=3 */
            if (e == 1) {
                /* nothing to add */
            } else if (e == 2) {
                add_exp(lpr, lex, &lcnt, 2, 1);
            } else {
                add_exp(lpr, lex, &lcnt, 2, e - 2);
            }
        } else {
            /* λ(p^e) = p^{e-1} * (p-1) */
            /* contribution from p^{e-1} */
            add_exp(lpr, lex, &lcnt, p, e - 1);

            /* contribution from (p-1) */
            uint64_t pf[64], pe[64];
            size_t pf_cnt;
            factor_u64(p - 1, pf, pe, &pf_cnt);
            for (size_t j = 0; j < pf_cnt; ++j) {
                add_exp(lpr, lex, &lcnt, pf[j], pe[j]);
            }
        }
    }

    /* Compute divisor counts */
    seq_t total = 1;      /* total number of divisors of λ(n)   */
    seq_t odd_total = 1;  /* number of odd divisors of λ(n)     */

    for (size_t i = 0; i < lcnt; ++i) {
        seq_t mult = (seq_t)(lex[i] + (uint64_t)1);
        total *= mult;
        if (lpr[i] != 2) {
            odd_total *= mult;
        }
    }

    if (total == odd_total) {
        return 0;    /* λ(n) has no factor 2 -> no even divisor */
    } else {
        return total - odd_total;
    }
}
