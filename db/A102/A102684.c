/*
 * A102684  Number of times the digit 9 appears in the decimal representations
 *          of all integers from 0 to n.
 * https://oeis.org/A102684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of digit '9's in decimal representations of all
 *            integers 0..n
 * Range:     u8 n <= 968, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t power = 1;
    while (power <= n)
    {
        idx_t high = (n / power) / 10;
        idx_t cur  = (n / power) % 10;
        idx_t low  = n % power;

        seq_t base = (seq_t)high * (seq_t)power;
        if (cur == 9)
            base += (seq_t)low + 1;

        total += base;

        if (power > n / 10)
            break;
        power *= 10;
    }
    return total;
}
