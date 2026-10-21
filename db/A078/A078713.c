/*
 * A078713  Sum of squares of the distances between successive divisors of n.
 * https://oeis.org/A078713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i} (d_{i+1} - d_i)^2, where d_1 < d_2 < … < d_k are
 *            the positive divisors of n
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* provides factor_u64 */

static seq_t *div_array = NULL;   /* reusable buffer for divisors   */
static size_t div_cap   = 0;
static size_t div_len   = 0;

/* Ensure capacity for at least 'need' elements.  */
static void ensure_div_cap(size_t need)
{
    if (need > div_cap) {
        size_t new_cap = need * 2 + 16;
        seq_t *tmp = (seq_t *)realloc(div_array, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure – terminate the program (undefined
               sequence value for the current call). */
            exit(EXIT_FAILURE);
        }
        div_array = tmp;
        div_cap   = new_cap;
    }
}

/* Comparator for qsort on seq_t values. */
static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* Recursive generation of all divisors from the prime
   factorization (primes and exponents).  */
static void gen_divs_rec(size_t idx, size_t cnt,
                         const uint64_t *pr, const uint64_t *ex,
                         seq_t cur)
{
    if (idx == cnt) {
        div_array[div_len++] = cur;
        return;
    }
    seq_t m = (seq_t)1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        gen_divs_rec(idx + 1, cnt, pr, ex, cur * m);
        m *= (seq_t)pr[idx];
    }
}

/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    /* factorization of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pf_cnt);

    /* total number of divisors */
    size_t need = 1;
    for (size_t i = 0; i < pf_cnt; ++i)
        need *= (size_t)(exps[i] + 1);

    ensure_div_cap(need);
    div_len = 0;

    gen_divs_rec(0, pf_cnt, primes, exps, (seq_t)1);

    /* sort divisors in ascending order */
    qsort(div_array, div_len, sizeof(seq_t), cmp_seq);

    /* compute sum of squares of successive gaps */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i + 1 < div_len; ++i) {
        seq_t diff = div_array[i + 1] - div_array[i];
        sum += diff * diff;
    }
    return sum;
}
/* -------------------------------------------------------------------- */
