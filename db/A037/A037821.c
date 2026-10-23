/*
 * A037821  Number of i such that d(i) > d(i-1), where Sum_{i=0..m} d(i)*6^i
 *          is the base-6 representation of n.
 * https://oeis.org/A037821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = #{ i : d_i > d_{i-1} } where d_i are the base‑6 digits of
 *            n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* no digits, no rises */

    /* first (least significant) digit */
    int prev_digit = (int)(n % 6);
    n /= 6;

    seq_t count = 0;
    while (n > 0) {
        int digit = (int)(n % 6);
        if (digit > prev_digit)
            count++;
        prev_digit = digit;
        n /= 6;
    }
    return count;
}
