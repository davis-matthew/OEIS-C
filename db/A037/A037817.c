/*
 * A037817  Number of i such that d(i) <= d(i-1), where Sum_{i=0..m} d(i)*10^i
 *          is the base-10 representation of n.
 * https://oeis.org/A037817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = |{ i : 1 ≤ i < number_of_decimal_digits(n) and *
 *            digit_i(n) ≤ digit_{i-1}(n) }| * where digits are counted from
 *            the least significant (10⁰) upward
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* for completeness; offset starts at 1 */

    seq_t count = 0;
    idx_t prev = n % 10;   /* least‑significant digit */
    n /= 10;
    while (n > 0) {
        idx_t curr = n % 10;     /* next digit towards the most significant */
        if (curr <= prev)
            ++count;
        prev = curr;
        n /= 10;
    }
    return count;
}
