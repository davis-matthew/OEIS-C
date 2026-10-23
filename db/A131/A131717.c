/*
 * A131717  Natural numbers A000027 with 6n+4 and 6n+5 terms swapped.
 * https://oeis.org/A131717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+1 if n mod 6 = 4; a(n) = n-1 if n mod 6 = 5; else a(n)
 *            = n
 * Range:     u8 n <= 255, u16 n <= 1002, u32 n <= 1002, u64 n <= 1002,
 *            u128 n <= 1002, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = (seq_t)n;
    idx_t r = n % 6;
    if (r == 4) {
        return v + 1;
    } else if (r == 5) {
        return v - 1;
    }
    return v;
}
