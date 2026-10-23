/*
 * A018653  Divisors of 800.
 * https://oeis.org/A018653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 800 in increasing order
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t divs[18];
static int init_done = 0;

static int cmp_seq(const void *p1, const void *p2)
{
    seq_t x = *(const seq_t *)p1;
    seq_t y = *(const seq_t *)p2;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

static void init_divisors(void)
{
    seq_t pw2[6];
    seq_t pw5[3];
    pw2[0] = (seq_t)1;
    for (idx_t i = 1; i <= 5; ++i)
        pw2[i] = pw2[i - 1] * (seq_t)2;
    pw5[0] = (seq_t)1;
    for (idx_t j = 1; j <= 2; ++j)
        pw5[j] = pw5[j - 1] * (seq_t)5;

    idx_t k = 0;
    for (idx_t i = 0; i <= 5; ++i)
    {
        for (idx_t j = 0; j <= 2; ++j)
        {
            divs[k++] = pw2[i] * pw5[j];
        }
    }
    qsort(divs, 18, sizeof(seq_t), cmp_seq);
    init_done = 1;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    if (!init_done)
        init_divisors();
    if (n > 18)
        return (seq_t)0;
    return divs[n - 1];
}
