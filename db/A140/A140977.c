/*
 * A140977  a(n) is the smallest integer > n that has the same number of
 *          (nonleading) zeros in its binary representation as n has.
 * https://oeis.org/A140977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the smallest integer > n that has the same number of
 *            nonleading zeros in its binary representation as n
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 87 terms through n = 977 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static unsigned zero_count(seq_t x)
{
    unsigned pop = 0;
    unsigned len = 0;
    while (x != (seq_t)0) {
        if ((x & (seq_t)1) != (seq_t)0)
            pop++;
        len++;
        x >>= 1;
    }
    return len - pop;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              // a(0) is irrelevant for the sequence

    unsigned target = zero_count((seq_t)n);   // required zero count

    seq_t m = (seq_t)n + (seq_t)1;          // start searching from n+1
    for (;;) {
        if (zero_count(m) == target)
            return m;
        ++m;                                 // next integer
    }
}
