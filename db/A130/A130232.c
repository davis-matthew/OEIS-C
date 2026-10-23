/*
 * A130232  a(1) = 0; a(n) = a(n-1) + (number of times the digit 0 has
 *          appeared in the sequence so far).
 * https://oeis.org/A130232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = a(n-1) + (number of times the digit 0 has appeared in the
 *            sequence so far); a(1) = 0
 * Range:     u8 n <= 55, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t count_zero_digits(seq_t x)
{
    if (x == 0) return 1;          /* "0" contains one zero digit */
    seq_t c = 0;
    while (x > 0) {
        if ((x % 10) == 0) ++c;    /* count zero digits */
        x /= 10;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;          /* a(1) = 0 */

    seq_t term  = 0;        /* current term a(k) */
    seq_t zeros = count_zero_digits(term);   /* zeros seen up to a(k) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t new_term = term + zeros;        /* a(i) = a(i-1) + zeros so far */
        zeros += count_zero_digits(new_term); /* update zeros for next step */
        term = new_term;
    }
    return term;            /* a(n) */
}
