/*
 * A018706  Divisors of 892.
 * https://oeis.org/A018706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth divisor of 892 in ascending order. */

#include <stddef.h>

static seq_t divisor_cache[6];
static size_t divisor_cnt = 0;

static void compute_divisors(void)
{
    if (divisor_cnt != 0) return;        /* already computed */

    const idx_t N = 892;                  /* constant to factorise */
    seq_t low[6];
    seq_t high[6];
    size_t lc = 0, hc = 0;

    for (idx_t d = 1; d * d <= N; ++d) {
        if (N % d == 0) {
            low[lc++] = (seq_t)d;
            if (d != N / d)
                high[hc++] = (seq_t)(N / d);
        }
    }

    /* low contains the small divisors in ascending order.
       high contains the paired divisors in descending order.
       Append a reversed high array to get full ascending list. */
    for (size_t i = 0; i < lc; ++i)
        divisor_cache[i] = low[i];
    for (size_t i = 0; i < hc; ++i)
        divisor_cache[lc + i] = high[hc - 1 - i];
    divisor_cnt = lc + hc;                /* should be 6 */
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for n == 0 */
    compute_divisors();
    if ((size_t)(n - 1) < divisor_cnt)
        return divisor_cache[n - 1];
    return (seq_t)0;                      /* undefined for n > 6 */
}
