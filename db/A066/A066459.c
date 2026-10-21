/*
 * A066459  Product of factorials of the digits of n.
 * https://oeis.org/A066459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, changed
 * Formula:   a(n) = product of factorials of the digits of n
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 88, u64 n <= 6998,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 1, ..., a(9) = 362880, a(10) = 1, etc. */
static const seq_t _fact_digits[10] = {
    (seq_t)1,  /* 0! */
    (seq_t)1,  /* 1! */
    (seq_t)2,  /* 2! */
    (seq_t)6,  /* 3! */
    (seq_t)24, /* 4! */
    (seq_t)120,/* 5! */
    (seq_t)720,/* 6! */
    (seq_t)5040,/* 7! */
    (seq_t)40320,/* 8! */
    (seq_t)362880 /* 9! */
};

seq_t a(idx_t n)
{
    if (n == 0)
        return _fact_digits[0];

    seq_t prod = 1;
    for (idx_t x = n; x > 0; x /= 10) {
        int d = x % 10;
        prod *= _fact_digits[d];
    }
    return prod;
}
