/*
 * A161184  Digital roots of highly composite numbers (A002182).
 * https://oeis.org/A161184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = A010888(A140645(n))
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n > 17 the digital root is always 9. */

seq_t a(idx_t n)
{
    /* If n is 0 or negative (outside OEIS offset) return 0 */
    if (n == 0)
        return 0;

    /* Digital roots of the first 17 highly composite numbers. */
    static const seq_t vals[17] = {
        1, 2, 4, 6, 3, 6, 9, 3, 6, 3, 9, 6, 9, 9, 3, 9, 6
    };

    if (n <= 17)
        return vals[n - 1];

    /* For all n > 17 the highly composite number is divisible by 9,
       so its digital root is 9. */
    return 9;
}
