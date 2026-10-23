/*
 * A144261  a(n) = smallest k such that k*n is a Niven (or Harshad) number.
 * https://oeis.org/A144261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 3040, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) = smallest k such that k*n is a Niven
   number (divisible by the sum of its decimal digits). */

static seq_t digit_sum(seq_t x)
{
    seq_t s = 0;
    while (x > 0) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

seq_t a(idx_t n)
{
    const seq_t sn = (seq_t)n;
    for (idx_t k = 1; ; ++k) {
        const seq_t prod = sn * (seq_t)k;
        const seq_t dsum = digit_sum(prod);
        if (dsum != 0 && prod % dsum == 0)
            return (seq_t)k;
    }
}
