/*
 * A018616  Divisors of 732.
 * https://oeis.org/A018616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 732
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* static state to cache the divisors of 732 */
static bool init_divisors_flag = false;
static size_t divisor_count = 0;
static seq_t divisors[12];

/* compute all divisors of 732 in ascending order */
static void init_divisors(void)
{
    if (init_divisors_flag)
        return;
    init_divisors_flag = true;

    const seq_t N = (seq_t)732;
    seq_t small[12];
    seq_t large[12];
    size_t cs = 0, cl = 0;

    for (idx_t i = 1; (seq_t)i * (seq_t)i <= N; ++i) {
        if (N % (seq_t)i == 0) {
            small[cs++] = (seq_t)i;
            if (i != N / (seq_t)i)         /* avoid double counting the square root */
                large[cl++] = N / (seq_t)i;
        }
    }

    divisor_count = cs + cl;
    for (size_t j = 0; j < cs; ++j)
        divisors[j] = small[j];
    for (size_t j = 0; j < cl; ++j)
        divisors[cs + j] = large[cl - 1 - j];
}

/* a(n) returns the n-th divisor of 732 (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* not used, but safe */

    if (!init_divisors_flag)
        init_divisors();

    if ((idx_t)(n - 1) >= divisor_count)
        return 0;                     /* out of range */

    return divisors[n - 1];
}
