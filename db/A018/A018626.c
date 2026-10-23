/*
 * A018626  Divisors of 750.
 * https://oeis.org/A018626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cmp_seq_t.h"

/* Sequence: divisors of 750 in increasing order (offset 1) */
static seq_t *divs = NULL;
static idx_t len = 0;

/* Recursively generate all divisors of 750. */
static void gen_divs(idx_t idx, seq_t prod, seq_t *buf, size_t *cnt,
                     const idx_t *p, const idx_t *e, idx_t max)
{
    if (idx == max) {
        buf[(*cnt)++] = prod;
        return;
    }
    seq_t cur = prod;
    for (idx_t i = 0; i <= e[idx]; ++i) {
        gen_divs(idx + 1, cur, buf, cnt, p, e, max);
        cur *= (seq_t)p[idx];
    }
}

/* Initialise the divisor table on first use. */
static void init_divisors(void)
{
    /* 750 = 2^1 * 3^1 * 5^3 */
    const idx_t primes[3] = {2, 3, 5};
    const idx_t exps[3]   = {1, 1, 3};

    seq_t temp[32];
    size_t cnt = 0;
    gen_divs(0, (seq_t)1, temp, &cnt, primes, exps, 3);

    len = (idx_t)cnt;
    divs = (seq_t *)malloc(len * sizeof(seq_t));
    for (idx_t i = 0; i < len; ++i) {
        divs[i] = temp[i];
    }
    qsort(divs, len, sizeof(seq_t), cmp_seq_t);
}

seq_t a(idx_t n)
{
    if (!divs) init_divisors();       /* first call: compute all divisors */
    if (n == 0) return (seq_t)0;      /* offset is 1; undefined below */
    idx_t idx = n - 1;
    return (idx < len) ? divs[idx] : (seq_t)0;
}
