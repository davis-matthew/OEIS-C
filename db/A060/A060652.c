/*
 * A060652  Orders of non-Abelian groups: n such that some group of order n is
 *          non-Abelian.
 * https://oeis.org/A060652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A060652 – An integer n is in the sequence iff for its prime
 *            factorisation n = ∏ p_i^e_i the following holds: • some exponent
 *            e_i > 2, or • there are distinct i, j with p_i^e_i ≡ 1 (mod p_j)
 * Range:     u8 n <= 156, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor.h"

/* test whether a number can be the order of a non‑Abelian group */
static bool is_non_abelian(uint64_t m)
{
    if (m < 6) return false;                 /* first term is 6 */

    static uint64_t primes[64], exps[64];
    size_t cnt;
    factor(m, primes, exps, &cnt);

    /* exponent larger than 2 guarantees a non‑Abelian group */
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] > 2) return true;

    /* congruence condition */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t pi = primes[i];
        uint64_t ei = exps[i];
        for (size_t j = 0; j < cnt; ++j) {
            if (i == j) continue;
            uint64_t pj = primes[j];

            /* compute pi^ei mod pj */
            uint64_t r = 1 % pj;
            for (uint64_t k = 0; k < ei; ++k)
                r = (r * pi) % pj;
            if (r == 1) return true;
        }
    }
    return false;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t used = 0, cap = 0;
    static uint64_t next = 5;        /* last tested integer */

    if (n == 0) return 0;          /* undefined, safe guard */

    while (used < n) {
        /* find next qualifying integer */
        do { ++next; } while (!is_non_abelian(next));

        /* grow cache if necessary */
        if (used == cap) {
            cap = cap ? cap * 2 : 16;
            terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
        }
        terms[used++] = (seq_t)next;
    }
    return terms[n - 1];
}
