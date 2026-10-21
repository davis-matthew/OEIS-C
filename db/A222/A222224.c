/*
 * A222224  In the number n, replace all (decimal) digits '1' with '7' and
 *          vice versa.
 * https://oeis.org/A222224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = replace each decimal digit 1 with 7 and each 7 with 1,
 *            leaving other digits unchanged
 * Range:     u8 n <= 99, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t number = (seq_t)n;
    seq_t result = 0;
    seq_t mul = 1;

    while (number > 0)
    {
        int d = (int)(number % 10);      /* current decimal digit */
        int nd;
        if (d == 1)
            nd = 7;
        else if (d == 7)
            nd = 1;
        else
            nd = d;
        result += (seq_t)nd * mul;
        number /= 10;
        mul *= 10;
    }
    return result;
}
