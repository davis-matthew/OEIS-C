/*
 * A193432  Number of divisors of n^2 + 1.
 * https://oeis.org/A193432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n^2 + 1 (i.e., a(n) = d(n^2+1))
 * Range:     u8 n <= 15, u16 n <= 1563, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;                            /* cast n to seq_t */
    seq_t m = nn * nn + 1;                   /* m = n^2 + 1 */

    /* Count divisors of m using trial division.  All operations are on
       seq_t, so the result fits in seq_t when it fits; otherwise the
       value will wrap naturally as per the type's limits. */

    seq_t dcount = 1;        /* multiplicative identity */
    seq_t temp   = m;        /* work on a copy of m */

    /* Handle factor 2 separately */
    if (temp % 2 == 0) {
        uint64_t exp = 0;    /* exponent of 2 */
        while (temp % 2 == 0) {
            temp /= 2;
            ++exp;
        }
        dcount *= (seq_t)(exp + 1);
    }

    /* Test odd divisors up to sqrt(temp) */
    seq_t i = 3;
    while (i <= temp / i) {          /* equivalent to i*i <= temp */
        if (temp % i == 0) {
            uint64_t exp = 0;
            while (temp % i == 0) {
                temp /= i;
                ++exp;
            }
            dcount *= (seq_t)(exp + 1);
        }
        i += 2;
    }

    /* If remainder > 1, it is a prime factor > sqrt(original m) */
    if (temp > 1) {
        dcount *= 2;                  /* exponent 1 => factor 2 */
    }

    return dcount;
}
