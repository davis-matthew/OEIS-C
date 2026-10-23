/*
 * A165725  Largest divisor of n coprime to 30. I.e., a(n) = max { k | gcd(n,
 *          k) = k and gcd(k, 30) = 1 }.
 * https://oeis.org/A165725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = n / (2^v2(n) * 3^v3(n) * 5^v5(n))
 * Range:     u8 n <= 222, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e., remove all factors 2, 3 and 5 from n to get the largest divisor coprime to 30 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;          /* convert index to the actual type */

    /* strip powers of 2 */
    while ((val % 2) == 0)
        val /= 2;

    /* strip powers of 3 */
    while ((val % 3) == 0)
        val /= 3;

    /* strip powers of 5 */
    while ((val % 5) == 0)
        val /= 5;

    return val;
}
