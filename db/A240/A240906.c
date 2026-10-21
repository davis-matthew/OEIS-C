/*
 * A240906  Maximal A050376-factors in factorization of factorials over
 *          distinct terms of A050376.
 * https://oeis.org/A240906
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 for n=2 3 for n=3 4 for n=4 5 for n=5 16 for n=6 81 for
 *            n=7 256 for n=8 a(n-1)^2 for n≥9 where ^ denotes exponentiation
 *            (squaring)
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 9, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(n) grows as 2^(2^(n-5)) for n≥8. */
seq_t a(idx_t n)
{
    /* Handle the first few terms directly. */
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)4;
    if (n == 5) return (seq_t)5;
    if (n == 6) return (seq_t)16;
    if (n == 7) return (seq_t)81;
    if (n == 8) return (seq_t)256;

    /* For n ≥ 9, use the recurrence a(n) = a(n-1)^2. */
    seq_t prev = (seq_t)256;      /* a(8) */
    /* If n has already been requested, skip the loop. */
    if (n <= 8) return prev;     /* safety, already handled above */

    /* Iteratively compute a(n) by repeated squaring. */
    for (idx_t i = 9; i <= n; ++i) {
        prev = prev * prev;
    }
    return prev;
}
