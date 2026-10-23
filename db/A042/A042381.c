/*
 * A042381  Denominators of continued fraction convergents to sqrt(717).
 * https://oeis.org/A042381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 13996798*a(n-12) - a(n-24) for n > 23 with initial
 *            values: a(0)=1,a(1)=1,a(2)=4,a(3)=9,
 *            a(4)=112,a(5)=121,a(6)=2048,a(7)=2169,
 *            a(8)=28076,a(9)=58321,a(10)=203039,a(11)=261360,
 *            a(12)=13793759,a(13)=14055119,a(14)=55959116,a(15)=125973351,
 *            a(16)=1567639328,a(17)=1693612679,a(18)=28665442192,...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Initial 24 terms */
    static const seq_t init_vals[24] = {
        (seq_t)1,        (seq_t)1,        (seq_t)4,          (seq_t)9,
        (seq_t)112,      (seq_t)121,      (seq_t)2048,       (seq_t)2169,
        (seq_t)28076,    (seq_t)58321,    (seq_t)203039,     (seq_t)261360,
        (seq_t)13793759, (seq_t)14055119, (seq_t)55959116,   (seq_t)125973351,
        (seq_t)1567639328, (seq_t)1693612679, (seq_t)28665442192,
        (seq_t)30359054871, (seq_t)392974100644,
        (seq_t)816307256159, (seq_t)2841895869121, (seq_t)3658203125280
    };

    /* If n is within the pre‑computed range, return directly */
    if (n < 24u) {
        return init_vals[(size_t)n];
    }

    /* A circular buffer holding the last 25 computed values.
       Index i in the original sequence maps to buf[i % 25] */
    seq_t buf[25];

    /* Load the first 24 values into the buffer */
    for (size_t i = 0; i < 24u; ++i) {
        buf[i] = init_vals[i];
    }

    /* Compute values from 24 up to n */
    for (size_t i = 24u; i <= (size_t)n; ++i) {
        /* Positions of the two dependencies */
        size_t idx12 = (i - 12u) % 25u;
        size_t idx24 = (i - 24u) % 25u;

        /* a(i) = 13996798 * a(i-12) - a(i-24) */
        seq_t term = (seq_t)13996798 * buf[idx12] - buf[idx24];
        buf[i % 25u] = term;
    }

    return buf[(size_t)n % 25u];
}
