/*
 * A074067  Zigzag modulo 5.
 * https://oeis.org/A074067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n (for n <= 2), a(n) = 5*floor(n/5) + 10*floor((n%5)/3) –
 *            (n%5) (for n > 2)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A074067 (Zigzag modulo 5). */
seq_t a(idx_t n)
{
    /* Handle the small initial segment explicitly. */
    if (n <= 2)
        return (seq_t)n;   /* a(1) = 1, a(2) = 2 */

    /* Work with seq_t arithmetic to avoid overflows on large builds. */
    seq_t N = (seq_t)n;          /* n itself as seq_t */
    seq_t d = N % (seq_t)5;      /* n mod 5 */
    seq_t q = N / (seq_t)5;      /* floor(n/5) */
    seq_t r = d / (seq_t)3;      /* floor((n%5)/3) – 0 or 1 because d < 5 */

    return (5 * q) + (10 * r) - d;
}
