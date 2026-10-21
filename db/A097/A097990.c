/*
 * A097990  A puzzle: reverse digits of n^2 + 10.
 * https://oeis.org/A097990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = reverse(n^2 + 10), with the special case a(0) = 10
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t reverse_digits(seq_t x)
{
    seq_t rev = (seq_t)0;
    const seq_t ten = (seq_t)10;
    while (x > (seq_t)0) {
        seq_t digit = x % ten;
        rev = rev * ten + digit;
        x = x / ten;
    }
    return rev;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)10;

    seq_t nn = (seq_t)n;
    seq_t val = nn * nn + (seq_t)10;
    return reverse_digits(val);
}
