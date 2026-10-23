/*
 * A182621  a(n) is the concatenation of the binary numbers that are the
 *          divisors of n written in base 2.
 * https://oeis.org/A182621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) = concatenation of the binary representations of all
 *            divisors of n (ascending), interpreted as a decimal integer
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 11, u64 n <= 23, u128 n <= 59,
 *            big any n
 * Helpers:   factor_u64, floor_log2_uint64, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/floor_log2_uint64.h"
#include "../utils/pow10.h"

/* Comparator for sorting divisors. */
static int cmp_divs(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* a(n) computes the OEIS sequence A182621. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, return 0 */

    /* Factorise n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pf_cnt);

    /* Generate all divisors (ascending). */
    seq_t divs[8192];
    size_t div_cnt = 1;
    divs[0] = 1;

    for (size_t i = 0; i < pf_cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t kmax = exps[i];
        size_t cur_cnt = div_cnt;
        seq_t pow = 1;
        for (uint64_t k = 1; k <= kmax; ++k) {
            pow = pow * p;                 /* p^k */
            for (size_t j = 0; j < cur_cnt; ++j) {
                divs[div_cnt++] = divs[j] * pow;
            }
        }
    }

    qsort(divs, div_cnt, sizeof(seq_t), cmp_divs);

    /* Concatenate binary strings as decimal digits. */
    seq_t res = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t d = (uint64_t)divs[i];
        unsigned bits = (unsigned)floor_log2_uint64(d) + 1;

        /* Compute 10^bits. */
        seq_t pow10bits = pow10(bits);

        /* Compute the decimal value of the binary representation of d. */
        seq_t bin_val = 0;
        for (int j = bits - 1; j >= 0; --j) {
            unsigned bit = (d >> j) & 1U;
            bin_val = bin_val * 10 + bit;
        }

        res = res * pow10bits + bin_val;
    }

    return res;
}
