/*
 * A229997  Numerator of d(k)/d(1) + d(k-1)/d(2) + ... + d(k)/d(1), where
 *          d(1),d(2),...,d(k) are the unitary divisors of n.
 * https://oeis.org/A229997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = the numerator of Σ_{i=1..k} d_{k-i+1}/d_i where d_1,…,d_k
 *            are the unitary divisors of n
 * Range:     u8 n <= 5, u16 n <= 37, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static int cmp_seq(const void *a, const void *b) {
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va > vb) - (va < vb);
}

/* generate all unitary divisors of n; sorted increasingly */
static seq_t *unitary_divisors(uint64_t n, size_t *cnt_out) {
    uint64_t temp = n;
    uint64_t prime_powers[16];
    size_t pp_cnt = 0;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t pp = 1;
            while (temp % p == 0) {
                pp *= p;
                temp /= p;
            }
            prime_powers[pp_cnt++] = pp;
        }
    }
    if (temp > 1)          /* remaining prime factor */
        prime_powers[pp_cnt++] = temp;

    /* start with divisor 1 */
    size_t div_cnt = 1;
    seq_t *divs = (seq_t *)malloc(sizeof(seq_t));
    divs[0] = 1;

    for (size_t i = 0; i < pp_cnt; ++i) {
        seq_t pp = prime_powers[i];
        size_t cur = div_cnt;
        divs = (seq_t *)realloc(divs, sizeof(seq_t) * (div_cnt + cur));
        for (size_t j = 0; j < cur; ++j) {
            divs[div_cnt++] = divs[j] * pp;
        }
    }

    qsort(divs, div_cnt, sizeof(seq_t), cmp_seq);
    *cnt_out = div_cnt;
    return divs;
}

/* add term (term_num/term_den) to (num/den) reducing during the addition */
static void add_fraction(seq_t *num, seq_t *den, seq_t term_num, seq_t term_den) {
    seq_t g = gcd(*den, term_den);
    seq_t den_div_g = *den / g;
    seq_t term_den_div_g = term_den / g;

    seq_t new_num = (*num) * term_den_div_g + term_num * den_div_g;
    seq_t new_den = den_div_g * term_den;
    seq_t g2 = gcd(new_num, new_den);
    *num = new_num / g2;
    *den = new_den / g2;
}

/* required function */
seq_t a(idx_t n) {
    size_t div_cnt;
    seq_t *divs = unitary_divisors((uint64_t)n, &div_cnt);

    seq_t N = (seq_t)n;
    seq_t num = 0;
    seq_t den = 1;

    for (size_t i = 0; i < div_cnt; ++i) {
        seq_t d = divs[i];
        seq_t d_sq = d * d;          /* d_i^2 */
        add_fraction(&num, &den, N, d_sq);
    }

    free(divs);
    return num;
}
