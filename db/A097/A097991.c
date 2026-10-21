/*
 * A097991  A puzzle: reverse digits of n^2 + 10.
 * https://oeis.org/A097991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = reverse(n^2 + 10)
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Reverses the decimal digits of a seq_t value. */
static seq_t reverse_digits(seq_t x)
{
    seq_t rev = 0;
    while (x != 0) {
        seq_t d = x % (seq_t)10;
        rev = rev * (seq_t)10 + d;
        x = x / (seq_t)10;
    }
    return rev;
}

/* OEIS sequence A097991: a(n) = reverse(n^2 + 10). */
seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;
    seq_t v  = sq + (seq_t)10;
    return reverse_digits(v);
}
