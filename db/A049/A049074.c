/*
 * A049074  Ulam's conjecture (steps to return n to 1 after division by 2 and,
 *          if needed, multiplication by 3 with 1 added).
 * https://oeis.org/A049074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: sum of the Collatz trajectory starting at n and ending when 1 is reached again. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t x = (seq_t)n;
    do {
        sum += x;
        if ((x & 1) == 0)
            x >>= 1;
        else
            x = x * 3 + 1;
    } while (x != 1);
    sum += 1;
    return sum;
}
