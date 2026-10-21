/*
 * A087302  Smallest nontrivial multiple of n whose digit sum is a multiple of
 *          digit sum of n. By nontrivial one means a(n) is not equal to n or
 *          (10^k)*n or a concatenation of n with itself.
 * https://oeis.org/A087302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest non‑trivial multiple k·n (k>1, k≠10^t, k≠10^d+1)
 *            with digit_sum(k·n) ≡ 0 (mod digit_sum(n)), where d=number of
 *            decimal digits of n
 * Range:     u8 n <= 18, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t digit_sum_seq(seq_t x) {
    seq_t sum = 0;
    while (x > 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n) {
    /* digit sum of n */
    seq_t ds_n = digit_sum_seq(n);

    /* number of decimal digits of n */
    unsigned int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        ++len;
        tmp /= 10;
    }

    /* 10^len */
    idx_t pow10d = 1;
    for (unsigned int i = 0; i < len; ++i)
        pow10d *= 10;
    /* concatenation multiplier k = 10^len + 1 */
    idx_t concat_k = pow10d + 1;

    /* search for the smallest admissible multiple */
    for (idx_t k = 2; ; ++k) {
        /* skip multipliers that are powers of 10 */
        idx_t t = k;
        while (t % 10 == 0)
            t /= 10;
        if (t == 1)                      /* k is a power of ten */
            continue;

        /* skip the concatenation of n with itself */
        if (k == concat_k)
            continue;

        seq_t m = k * n;
        seq_t ds_m = digit_sum_seq(m);
        if (ds_m % ds_n == 0)
            return m;
    }
}
