/*
 * A037820  Number of i such that d(i) > d(i-1), where Sum_{i=0..m} d(i)*5^i
 *          is the base-5 representation of n.
 * https://oeis.org/A037820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of indices i such that d(i) > d(i-1) for the
 *            base‑5 digits d(i) of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* safety for n = 0, not used in OEIS */
    idx_t temp = n;
    idx_t prev = temp % 5;        /* least significant digit */
    temp   /= 5;
    seq_t count = 0;

    while (temp > 0) {
        idx_t curr = temp % 5;     /* next digit to the left */
        if (curr > prev)
            ++count;
        prev = curr;
        temp /= 5;
    }
    return count;
}
