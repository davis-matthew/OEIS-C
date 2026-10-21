/*
 * A226171  Smallest base in which n is not Niven (or zero if n is Niven in
 *          every base).
 * https://oeis.org/A226171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base b >= 2 such that n is not Niven in base b,
 *            0 if n is Niven in every base
 * Range:     u8 n <= 757, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t digit_sum_seq_t(seq_t n, seq_t base)
{
    seq_t sum = 0;
    while (n > 0) {
        sum += n % base;
        n /= base;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    for (idx_t b = 2; b <= n; ++b) {
        seq_t base = (seq_t)b;
        seq_t s = digit_sum_seq_t(n_seq, base);
        if (s == 0) continue;               /* safety, should not happen */
        if (n_seq % s != 0) return base;      /* first base where not Niven */
    }
    return 0;                                 /* Niven in every base */
}
