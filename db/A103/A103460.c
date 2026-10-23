/*
 * A103460  a(n) = 9^n + 1 - 0^n.
 * https://oeis.org/A103460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9^n + 1 - 0^n (i.e. a(0)=1, a(n)=9^n+1 for n>0)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)9;
    idx_t e = n;

    while (e > 0) {
        if (e & 1)
            result *= base;
        base *= base;
        e >>= 1;
    }
    return result + (seq_t)1;
}
