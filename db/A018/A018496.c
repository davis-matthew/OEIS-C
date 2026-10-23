/*
 * A018496  Divisors of 516.
 * https://oeis.org/A018496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 516 (in increasing order)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating all divisors of 516 once on the first call. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices before the offset (n < 1) or beyond the number of divisors. */
    if (n < (idx_t)1)
        return (seq_t)0;

    /* Fixed number whose divisors are enumerated. */
    const seq_t N = (seq_t)516;

    /* Static buffer to hold the 12 divisors of 516. */
    static seq_t divs[12];
    static size_t div_cnt = 0;
    static int initialized = 0;

    if (!initialized) {
        /* Enumerate divisors of N in ascending order. */
        for (seq_t d = (seq_t)1; d <= N; ++d) {
            if (N % d == (seq_t)0)
                divs[div_cnt++] = d;
        }
        initialized = 1;
    }

    if (n <= (idx_t)div_cnt)
        return divs[(size_t)(n - 1)];
    else
        return (seq_t)0;
}
