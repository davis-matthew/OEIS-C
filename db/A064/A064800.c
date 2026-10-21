/*
 * A064800  n plus the number of its prime factors: a(n) = n + A001222(n).
 * https://oeis.org/A064800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + Ω(n) where Ω(n) is the total number of prime factors
 *            of n counted with multiplicity
 * Range:     u8 n <= 251, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case n=0 (offset is 1, but just in case). */
    if (n == 0) return 0;

    seq_t omega = 0;          /* number of prime factors counted with multiplicity */
    uint64_t temp = (uint64_t)n;

    /* Count factors of 2 */
    while ((temp & 1ULL) == 0) {
        ++omega;
        temp >>= 1;           /* divide by 2 */
    }

    /* Count odd factors */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        while (temp % p == 0) {
            ++omega;
            temp /= p;
        }
    }

    /* If anything remains it's a prime factor larger than sqrt(original n) */
    if (temp > 1) {
        ++omega;
    }

    /* a(n) = n + Ω(n) */
    return (seq_t)n + omega;
}
