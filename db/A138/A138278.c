/*
 * A138278  Sequence identical to its third differences in absolute values.
 * https://oeis.org/A138278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-3) for n>=6, with initial terms a(0)=1, a(1)=2,
 *            a(2)=4, a(3)=8, a(4)=16, a(5)=24
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base terms for the first six indices. */
    static const seq_t base[6] = {
        (seq_t)1, (seq_t)2, (seq_t)4,
        (seq_t)8, (seq_t)16, (seq_t)24
    };

    if (n < 6)
        return base[n];

    /* Reduce n by multiples of 3 until it is < 6,
       counting how many times we subtracted 3. */
    idx_t t = n;
    idx_t steps = 0;
    while (t >= 6) {
        t -= 3;
        ++steps;
    }

    /* Fast exponentiation of 5^steps. */
    seq_t pow5 = (seq_t)1;
    seq_t mul   = (seq_t)5;
    while (steps) {
        if (steps & 1) pow5 *= mul;
        mul *= mul;
        steps >>= 1;
    }

    return base[t] * pow5;
}
