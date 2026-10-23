/*
 * A018766  Divisors of 999.
 * https://oeis.org/A018766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th divisor of 999 in ascending order
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Comparator for qsort on seq_t values */
static int cmp_seq(const void *p, const void *q)
{
    seq_t a = *(const seq_t *)p;
    seq_t b = *(const seq_t *)q;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

static seq_t divisors[8];
static size_t div_cnt = 0;
static bool initialized = false;

static void init_divisors(void)
{
    /* 999 = 3^3 * 37^1 */
    const idx_t primes[2]  = {3, 37};
    const idx_t exps[2]    = {3, 1};
    seq_t pow0[4] = {1, 0, 0, 0};
    seq_t pow1[2] = {1, 0};

    /* compute powers of 3 */
    pow0[0] = (seq_t)1;
    for (idx_t i = 1; i <= exps[0]; ++i)
        pow0[i] = pow0[i - 1] * (seq_t)primes[0];

    /* compute powers of 37 */
    pow1[0] = (seq_t)1;
    for (idx_t i = 1; i <= exps[1]; ++i)
        pow1[i] = pow1[i - 1] * (seq_t)primes[1];

    div_cnt = 0;
    for (idx_t e0 = 0; e0 <= exps[0]; ++e0)
    {
        for (idx_t e1 = 0; e1 <= exps[1]; ++e1)
        {
            divisors[div_cnt++] = pow0[e0] * pow1[e1];
        }
    }

    /* sort ascending */
    qsort(divisors, div_cnt, sizeof(seq_t), cmp_seq);
}

seq_t a(idx_t n)
{
    if (!initialized)
    {
        init_divisors();
        initialized = true;
    }
    if (n == 0 || n > div_cnt)
        return (seq_t)0;
    return divisors[n - 1];
}
