/*
 * A189202  Let s_k(n) denote the sum of digits of n in base k. Then a(n) is
 *          the smallest m>0 such that both s_2(m*(n-1)) and
 *          s_n(2*m*(n-1))/(n-1) are even, or a(n)=0, if such m does not
 *          exist.
 * https://oeis.org/A189202
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m>0 such that s_2(m*(n-1)) is even and
 *            [s_n(2*m*(n-1))/(n-1)] is even (return 0 if no such m exists)
 * Range:     u8 n <= 11, u16 n <= 185, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* helper: sum of digits of val in given base */
static seq_t sum_digits_base(seq_t val, unsigned base)
{
    seq_t sum = 0;
    while (val != 0) {
        sum += val % base;
        val /= base;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;             /* out of defined range */
    seq_t n_minus1 = (seq_t)(n - 1);  /* (n-1) as seq_t */

    /* search for smallest m */
    /* safety limit to avoid infinite loops (unreliable but harmless) */
    const idx_t LIMIT = 10000000ULL;
    idx_t iterations = 0;

    for (seq_t m = 1; iterations < LIMIT; ++m, ++iterations) {
        seq_t x          = m * n_minus1;          /* m*(n-1) */
        seq_t sum2      = sum_digits_base(x, 2);   /* s_2(x) */

        if ((sum2 & 1) != 0)    /* sum2 odd -> fail */
            continue;

        seq_t two_x      = x + x;                  /* 2*x */
        seq_t sumn      = sum_digits_base(two_x, (unsigned)n); /* s_n(2x) */

        if (sumn % n_minus1 != 0)   /* not divisible by (n-1) -> fail */
            continue;

        seq_t quotient   = sumn / n_minus1;        /* sumn/(n-1) */
        if ((quotient & 1) != 0)   /* quotient odd -> fail */
            continue;

        return m;   /* found smallest m */
    }

    return 0;  /* no m found within limit */
}
