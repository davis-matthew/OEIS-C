/*
 * A134725  Concatenation of next n squares of positive integers.
 * https://oeis.org/A134725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concatenation of the squares of n consecutive integers
 *            starting from k = 1 + n(n-1)/2 (i.e. a(n) = concat_{i=0}^{n-1} (
 *            (k+i)^2 )
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static unsigned digits(seq_t x)
{
    unsigned d = 0;
    do {
        d++;
        x /= 10;
    } while (x != 0);
    return d;
}

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t k = n_seq * (n_seq - 1) / 2 + 1;

    seq_t result = 0;

    for (seq_t i = 0; i < n_seq; ++i) {
        seq_t base = k + i;
        seq_t sq   = base * base;

        unsigned d = digits(sq);

        seq_t pow10 = 1;
        for (unsigned j = 0; j < d; ++j)
            pow10 *= 10;

        result = result * pow10 + sq;
    }
    return result;
}
