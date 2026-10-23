/*
 * A037816  Number of i such that d(i) <= d(i-1), where Sum_{i=0..m} d(i)*9^i
 *          is the base-9 representation of n.
 * https://oeis.org/A037816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of indices i≥1 such that the i‑th base‑9 digit of
 *            n (counting from 0) is ≤ the (i–1)‑th digit
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;           /* result */
    if (n == 0)                /* not used for offset 1, but safe */
        return count;

    unsigned char prev = (unsigned char)(n % 9);
    n = n / 9;
    while (n > 0) {
        unsigned char cur = (unsigned char)(n % 9);
        if (cur <= prev)
            ++count;
        prev = cur;
        n = n / 9;
    }
    return count;
}
