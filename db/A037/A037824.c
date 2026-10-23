/*
 * A037824  Number of i such that d(i) > d(i-1), where Sum_{i=0..m} d(i)*9^i
 *          is the base-9 representation of n.
 * https://oeis.org/A037824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of i such that d(i) > d(i-1), where the base‑9
 *            digits of n are d(0)..d(m)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0)    /* defined for n>=1 but defensive */
        return 0;

    /* least significant digit */
    idx_t prev = n % 9;
    n /= 9;

    while (n > 0) {
        idx_t digit = n % 9;
        if (digit > prev)
            count++;
        prev = digit;
        n /= 9;
    }
    return count;
}
