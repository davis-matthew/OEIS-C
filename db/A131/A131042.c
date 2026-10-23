/*
 * A131042  Natural numbers A000027 with 6n+3 and 6n+4 terms swapped.
 * https://oeis.org/A131042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 1 if n mod 6 = 3; a(n) = n - 1 if n mod 6 = 4;
 *            otherwise a(n) = n
 * Range:     u8 n <= 254, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)n;
    idx_t mod = n % (idx_t)6;
    if (mod == (idx_t)3) {
        res++;
    } else if (mod == (idx_t)4) {
        res--;
    }
    return res;
}
