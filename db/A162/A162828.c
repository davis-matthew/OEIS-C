/*
 * A162828  Positive numbers n such that 2*90*n/(90+n) are integers.
 * https://oeis.org/A162828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) are the positive integers n such that 2*90*n/(90+n) is an
 *            integer. Let d = n + 90. The divisibility condition becomes 180n
 *            divisible by d, which simplifies to d | 16200. Therefore a(n) =
 *            d - 90 where d > 90 and d is a divisor of 16200. The sequence is
 *            the sorted list of those values of n. It...
 * Range:     u8 n <= 1, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static bool initialized = false;
static seq_t seq[60];      /* 60 Divisors of 16200, enough space */
static idx_t seq_len = 0;

static void init_sequence(void) {
    if (initialized)
        return;
    initialized = true;

    const idx_t N = 16200;
    idx_t divs[120]; /* temporary store for divisors (including both sides) */
    idx_t div_cnt = 0;

    for (idx_t i = 1; i * i <= N; ++i) {
        if (N % i == 0) {
            divs[div_cnt++] = i;
            if (i != N / i)
                divs[div_cnt++] = N / i;
        }
    }

    /* collect divisors greater than 90 and subtract 90 */
    for (idx_t i = 0; i < div_cnt; ++i) {
        if (divs[i] > 90) {
            seq[seq_len++] = (seq_t)(divs[i] - 90);
        }
    }

    /* simple insertion sort (seq_len <= 60) */
    for (idx_t i = 1; i < seq_len; ++i) {
        seq_t key = seq[i];
        idx_t j = i;
        while (j > 0 && seq[j - 1] > key) {
            seq[j] = seq[j - 1];
            --j;
        }
        seq[j] = key;
    }
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;            /* offset is 1, undefined for n<1 */
    init_sequence();
    if (n > seq_len)
        return 0;            /* out of range; caller may treat as 0 */
    return seq[n - 1];
}
