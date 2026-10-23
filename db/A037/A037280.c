/*
 * A037280  If n is composite replace n with the concatenation of its
 *          nontrivial divisors [ A037279 ] then divide out any factors of 2.
 * https://oeis.org/A037280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = odd part of * the concatenation of all non‑trivial
 *            divisors of n * (if n is composite; otherwise n itself). * For
 *            odd part, repeatedly divide by 2 until odd
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   cmp_uint64, digit_len, factor_u64, is_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>           /* qsort, malloc, realloc, free */

/* Utilities from the build */
#include "../utils/is_prime.h"      /* bool is_prime(idx_t n) */
#include "../utils/factor_u64.h"    /* void factor_u64(uint64_t n, uint64_t *primes,
                                      uint64_t *exps, size_t *cnt) */
#include "../utils/pow10.h"         /* seq_t pow10(idx_t exp) */
#include "../utils/digit_len.h"     /* int digit_len(seq_t x) */
#include "../utils/cmp_uint64.h"

/* Compare two uint64_t values for qsort */

/* Compute a(n) as described */
seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;        /* n fits in uint64_t */
    seq_t value;

    if (nn == 1 || is_prime(nn)) {
        /* For prime or 1, start with n itself */
        value = (seq_t)nn;
    } else {
        /* composite: compute all divisors */
        uint64_t primes[32], exps[32];
        size_t factor_cnt = 0;
        factor_u64(nn, primes, exps, &factor_cnt);

        /* generate all divisors (including 1 and n) */
        uint64_t divs[1024];
        size_t div_cnt = 1;
        divs[0] = 1;

        for (size_t i = 0; i < factor_cnt; ++i) {
            uint64_t p = primes[i];
            uint64_t power = 1;
            size_t current = div_cnt;
            for (int e = 1; e <= (int)exps[i]; ++e) {
                power *= p;
                for (size_t j = 0; j < current; ++j) {
                    divs[div_cnt++] = divs[j] * power;
                }
            }
        }

        /* sort divisors */
        qsort(divs, div_cnt, sizeof(uint64_t), cmp_uint64);

        /* concatenate non‑trivial divisors */
        value = 0;
        for (size_t i = 0; i < div_cnt; ++i) {
            uint64_t d = divs[i];
            if (d == 1 || d == nn)
                continue; /* skip 1 and n */
            seq_t dseq = (seq_t)d;
            unsigned int dig = (unsigned int)digit_len(dseq);
            value = value * pow10(dig) + dseq;
        }
    }

    /* remove all factors of 2 */
    while ((value & 1) == 0) {   /* value % 2==0 using bit test */
        value = value / 2;
    }
    return value;
}
