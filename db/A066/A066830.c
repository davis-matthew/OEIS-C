/*
 * A066830  a(n) = lcm(n+1, n-1).
 * https://oeis.org/A066830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n + 1, n - 1) (for n ≥ 1)
 * Range:     u8 n <= 17, u16 n <= 257, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t seq_gcd(seq_t a, seq_t b)
/* Euclidean algorithm for seq_t type. */
{
    while (b != 0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
/* Computes the least common multiple of (n+1) and (n-1). */
{
    /* Convert the index to the arithmetic type used for terms. */
    seq_t np1 = (seq_t)n + 1;   /* n + 1 */
    seq_t nm1 = (seq_t)n - 1;   /* n - 1 */

    /* Determine the greatest common divisor. */
    seq_t g = seq_gcd(np1, nm1);

    /* lcm = (n+1)/g * (n-1)   (order chosen to keep intermediate small). */
    seq_t l = (np1 / g) * nm1;

    return l;
}
