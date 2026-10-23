/*
 * A187207  Irregular triangle read by rows in which row n lists the
 *          k=A000005(n) divisors of n in decreasing order, followed by the
 *          lists of their absolute differences up to order k-1.
 * https://oeis.org/A187207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 6514, u16 n <= 8567, u32 n <= 8567, u64 n <= 8567,
 *            u128 n <= 8567, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A187207: irregular triangle read by rows. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Utility: factorization, returns number of distinct prime factors in *cnt.
   (Defined in ../utils/factor_u64.h) */
#include "../utils/factor_u64.h"

/* Comparator for descending order of uint64_t values. */
static int cmp_uint64_desc(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    if (x < y) return 1;
    if (x > y) return -1;
    return 0;
}

/* Recursive generation of all divisors of a number given its prime
   factorization. */
static void gen_divs(const uint64_t *primes, const uint64_t *exps,
                     size_t pf_cnt, size_t idx, uint64_t current,
                     uint64_t *out, size_t *pos)
{
    if (idx == pf_cnt) {           /* all primes processed */
        out[(*pos)++] = current;
        return;
    }
    uint64_t p = primes[idx];
    uint64_t exp = exps[idx];
    uint64_t val = current;
    for (uint64_t k = 0; k <= exp; ++k) {
        gen_divs(primes, exps, pf_cnt, idx + 1, val, out, pos);
        val *= p;                  /* multiply by p for next exponent level */
    }
}

seq_t a(idx_t n)
{
    /* Find the row that contains the n‑th term. */
    idx_t row = 1;
    unsigned __int128 cumulative = 0;      /* cumulative count of terms so far */
    unsigned __int128 target = n;         /* target index as 128 bit for safety */

    while (1) {
        /* Factorize 'row' to obtain divisor count */
        uint64_t primes[20];
        uint64_t exps[20];
        size_t pf_cnt = 0;
        factor_u64((uint64_t)row, primes, exps, &pf_cnt);

        size_t d = 1;
        for (size_t i = 0; i < pf_cnt; ++i)
            d *= (size_t)(exps[i] + 1);

        unsigned __int128 row_len = (unsigned __int128)d * (d + 1) / 2;

        if (cumulative + row_len >= target)
            break;

        cumulative += row_len;
        ++row;
    }

    /* Offset within the found row, zero‑based */
    idx_t offset = (idx_t)(target - cumulative - 1);

    /* Re‑factorize the row to generate its divisors. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)row, primes, exps, &pf_cnt);

    size_t d = 1;
    for (size_t i = 0; i < pf_cnt; ++i)
        d *= (size_t)(exps[i] + 1);

    uint64_t *divs = (uint64_t *)malloc(d * sizeof(uint64_t));
    size_t pos = 0;
    gen_divs(primes, exps, pf_cnt, 0, 1ULL, divs, &pos);

    /* Ensure we generated all divisors */
    divs = (uint64_t *)realloc(divs, pos * sizeof(uint64_t));
    d = pos;

    /* Sort divisors in descending order */
    qsort(divs, d, sizeof(uint64_t), cmp_uint64_desc);

    /* Iteratively produce difference lists until we reach the offset. */
    uint64_t *cur = divs;
    size_t cur_len = d;
    while (offset >= cur_len) {
        offset -= (idx_t)cur_len;
        if (cur_len == 1) break;    /* should not happen because cur_len == 1
                                      has only one element, so offset must be 0 */

        size_t next_len = cur_len - 1;
        uint64_t *next = (uint64_t *)malloc(next_len * sizeof(uint64_t));
        for (size_t i = 0; i < next_len; ++i) {
            uint64_t a1 = cur[i];
            uint64_t a2 = cur[i + 1];
            next[i] = (a1 > a2) ? a1 - a2 : a2 - a1;
        }
        free(cur);
        cur = next;
        cur_len = next_len;
    }

    uint64_t result = cur[offset];
    free(cur);
    return (seq_t)result;
}
