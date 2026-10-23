/*
 * A018451  Divisors of 430.
 * https://oeis.org/A018451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th smallest divisor of 430 (sorted ascending)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static seq_t divisors[8];
static bool initialized = false;

static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

static void generate_divisors_rec(size_t pos, seq_t current,
                                  const uint64_t *primes,
                                  const uint64_t *exps,
                                  size_t cnt,
                                  seq_t *out,
                                  size_t *idx)
{
    if (pos == cnt) {
        out[(*idx)++] = current;
        return;
    }
    seq_t value = 1;
    for (uint64_t k = 0; k <= exps[pos]; ++k) {
        generate_divisors_rec(pos + 1, current * value,
                              primes, exps, cnt, out, idx);
        value *= (seq_t)primes[pos];
    }
}

static void init_divisors(void)
{
    if (initialized)
        return;

    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt;

    factor_u64(430, primes, exps, &cnt);

    size_t num_divs = 1;
    for (size_t i = 0; i < cnt; ++i)
        num_divs *= (size_t)(exps[i] + 1);

    seq_t *tmp = (seq_t *)malloc(num_divs * sizeof(seq_t));
    if (!tmp)
        return; /* allocation failed – unlikely for such a small array */

    size_t idx = 0;
    generate_divisors_rec(0, (seq_t)1, primes, exps, cnt, tmp, &idx);

    qsort(tmp, num_divs, sizeof(seq_t), cmp_seq);

    for (size_t i = 0; i < num_divs; ++i)
        divisors[i] = tmp[i];

    free(tmp);
    initialized = true;
}

seq_t a(idx_t n)
{
    init_divisors();

    if (n == 0)
        return (seq_t)0; /* undefined below offset, but safety */

    if (n <= 8)
        return divisors[(size_t)(n - 1)];
    return (seq_t)0; /* all other indices are undefined for this sequence */
}
