/*
 * A084008  a(n) = T(n) concatenated with reverse(T(n)) divided by 11, where
 *          T(n) is the n-th triangular number.
 * https://oeis.org/A084008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (T(n) * 10^digits(T(n)) + reverse(T(n))) / 11, where T(n)
 *            = n*(n+1)/2 is the n‑th triangular number
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t reverse_decimal(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + (x % 10);
        x   = x / 10;
    }
    return rev;
}

static unsigned int digits_of(seq_t x)
{
    unsigned int d = 0;
    if (x == 0)
        return 1;          /* treat 0 as having one digit */
    while (x > 0) {
        x   = x / 10;
        d++;
    }
    return d;
}

seq_t a(idx_t n)
{
    /* Compute the triangular number T(n) */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Reverse its decimal representation */
    seq_t rev_tri = reverse_decimal(tri);

    /* Number of decimal digits in T(n) */
    unsigned int d = digits_of(tri);

    /* 10^d as a seq_t */
    seq_t pow10 = 1;
    for (unsigned int i = 0; i < d; ++i)
        pow10 *= 10;

    /* Concatenate T(n) and its reverse, then divide by 11 */
    seq_t concatenated = tri * pow10 + rev_tri;
    return concatenated / 11;
}
