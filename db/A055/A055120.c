/*
 * A055120  Digital complement of n (replace each nonzero digit d with 10-d).
 * https://oeis.org/A055120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nice, nonn
 * Formula:   a(n) = replace each non‑zero decimal digit d of n by (10-d)
 *            keeping zeros unchanged. This is equivalent to the recurrence
 *            a(10*n) = 10*a(n) a(10*n+j) = 10*a(n) + (10-j) (1 <= j <= 9)
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result  = 0;
    seq_t mult    = 1;
    idx_t tmp = n;
    while (tmp != 0)
    {
        unsigned digit = tmp % 10;
        tmp /= 10;
        if (digit != 0)
            result += (seq_t)(10 - digit) * mult;
        mult *= 10;
    }
    return result;
}
