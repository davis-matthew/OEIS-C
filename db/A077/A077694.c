/*
 * A077694  a(n) = triangular number whose index is the concatenation of the
 *          first n natural numbers.
 * https://oeis.org/A077694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (k*(k+1))/2, where k = concatenation of 1,2,...,n (OEIS
 *            A007908)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int digits_of_idx(idx_t x)
{
    int d = 0;
    do {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

static seq_t pow10_int(int e)
{
    seq_t r = 1;
    while (e-- > 0) {
        r *= 10;
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t concatenated = 0;
    for (idx_t i = 1; i <= n; ++i) {
        int d = digits_of_idx(i);
        concatenated = concatenated * pow10_int(d) + (seq_t)i;
    }
    return concatenated * (concatenated + 1) / 2;
}
