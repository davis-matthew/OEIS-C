/*
 * A080256  Sum of numbers of distinct and of all prime factors of n.
 * https://oeis.org/A080256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Omega(n) + omega(n) (sum of total and distinct prime
 *            factors)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;          /* by definition a(1) = 0 */

    /* we will factor n and count */
    idx_t temp = n;
    seq_t omega_total = 0;    /* total number of prime factors (with multiplicity)  */
    seq_t omega_distinct = 0; /* number of distinct prime factors              */

    /* factor out 2 */
    if ((temp & 1) == 0) {
        omega_distinct++;
        while ((temp & 1) == 0) {
            temp >>= 1;
            omega_total++;
        }
    }

    /* factor odd divisors up to sqrt(temp) */
    for (idx_t d = 3; d <= temp / d; d += 2) {
        if (temp % d == 0) {
            omega_distinct++;
            while (temp % d == 0) {
                temp /= d;
                omega_total++;
            }
        }
    }

    /* if something remains, it is a prime larger than sqrt(original n) */
    if (temp > 1) {
        omega_distinct++;
        omega_total++;
    }

    return omega_total + omega_distinct;
}
