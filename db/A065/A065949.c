/*
 * A065949  Bessel polynomial {y_n}'''(0).
 * https://oeis.org/A065949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)(n-1)n(n+1)(n+2)(n+3)/8 for n ≥ 3, 0 otherwise
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 55, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    seq_t t1 = (seq_t)(n - 2);
    seq_t t2 = (seq_t)(n - 1);
    seq_t t3 = (seq_t)n;
    seq_t t4 = (seq_t)(n + 1);
    seq_t t5 = (seq_t)(n + 2);
    seq_t t6 = (seq_t)(n + 3);

    seq_t prod = t1 * t2;
    prod *= t3;
    prod *= t4;
    prod *= t5;
    prod *= t6;

    return prod / (seq_t)8;
}
