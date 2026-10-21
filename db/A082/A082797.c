/*
 * A082797  Smallest multiple of n beginning with 7.
 * https://oeis.org/A082797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = minimal positive multiple of n whose decimal
 *            representation starts with 7. The algorithm tries increasing
 *            numbers of digits until a suitable multiple is found
 * Range:     u8 n <= 15, u16 n <= 1142, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;          /* undefined, but guard against divide by zero */
    /* number of digits of n */
    idx_t digits = 1;
    idx_t tmp = n;
    while (tmp >= 10) {
        tmp /= 10;
        ++digits;
    }

    /* power = 10^{digits-1} */
    seq_t power = (seq_t)1;
    for (idx_t i = 1; i < digits; ++i) {
        power *= (seq_t)10;
    }

    seq_t n_seq = (seq_t)n;
    for (idx_t L = digits; ; ++L) {              /* L = number of digits of the candidate multiple */
        seq_t lower = (seq_t)7 * power;          /* 7 * 10^{L-1} */
        seq_t upper = (seq_t)8 * power;          /* 8 * 10^{L-1} */

        /* ceil( lower / n ) */
        seq_t k_min = (lower + n_seq - 1) / n_seq;
        seq_t m = k_min * n_seq;                /* candidate multiple */

        if (m < upper)                         /* first digit is 7 */
            return m;

        power *= (seq_t)10;                     /* next power of ten for larger L */
    }
}
