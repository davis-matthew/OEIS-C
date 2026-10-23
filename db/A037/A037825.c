/*
 * A037825  Number of i such that d(i) > d(i-1), where Sum_{i=0..m} d(i)*10^i
 *          is the base-10 representation of n.
 * https://oeis.org/A037825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of indices i ≥ 1 such that the i-th decimal digit
 *            of n (counted from the least significant digit) is greater than
 *            the previous digit
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For single-digit numbers there are no adjacent pairs. */
    if (n < 10)
        return 0;

    /* Extract the least significant digit. */
    uint8_t prev = n % 10;
    n /= 10;

    seq_t count = 0;
    while (n > 0) {
        uint8_t cur = n % 10;
        if (cur > prev)
            count++;
        prev = cur;
        n /= 10;
    }
    return count;
}
