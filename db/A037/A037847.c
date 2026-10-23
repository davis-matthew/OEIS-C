/*
 * A037847  a(n)=Sum{d(i-1)-d(i): d(i)<d(i-1), i=0,1,...,m}, where
 *          Sum{d(i)*6^i: i=0,1,...,m} is the base 6 representation of n.
 * https://oeis.org/A037847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum over i ≥ 1 of (d(i-1) - d(i)) when d(i) < d(i-1),
 *            where d(i) are the base-6 digits of n (least significant digit
 *            is d(0))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for the offset index 0 (though a(1) is the first term). */
    if (n == 0)
        return 0;

    seq_t sum = 0;

    /* Last digit is the least significant digit. */
    idx_t last_digit = n % 6;
    n /= 6;

    while (n > 0) {
        idx_t next_digit = n % 6;
        n /= 6;

        if (next_digit < last_digit)
            sum += (seq_t)(last_digit - next_digit);

        last_digit = next_digit;
    }

    return sum;
}
