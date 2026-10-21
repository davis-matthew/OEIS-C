/*
 * A065335  3-exponents to represent 3-smooth numbers (A065332).
 * https://oeis.org/A065335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = log_3(n / A006519(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here A006519(n) is the largest divisor of n containing no factor 3.
   So a(n) equals the exponent of 3 in n when n has no prime factors other
   than 2 and 3; otherwise it is 0.  This matches OEIS A065335. */

seq_t a(idx_t n)
{
    /* Handle n == 0 gracefully (offset is 1). */
    if (n == 0)
        return (seq_t)0;

    idx_t temp = n;
    idx_t exp3  = 0;

    /* Count factors of 3. */
    while (temp % 3 == 0) {
        temp /= 3;
        exp3++;
    }

    /* Remove all remaining factors of 2. */
    while (temp % 2 == 0) {
        temp /= 2;
    }

    /* If after removing 2's and 3's we are left with 1, n was 3‑smooth. */
    if (temp == 1) {
        return (seq_t)exp3;
    } else {
        return (seq_t)0;
    }
}
