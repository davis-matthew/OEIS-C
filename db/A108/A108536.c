/*
 * A108536  Maximal "digit" in position n from right of numbers in "base
 *          American money" representation (A080897).
 * https://oeis.org/A108536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, fini, full, nonn, base
 * Formula:   a(n) = ⌊(d_{n+1} - 1)/d_n⌋ with denominations d =
 *            {1,5,10,25,50,100,200,500,1000,2000,5000,10000,50000,100000,500000,1000000,10000000}
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t denom[] = {
        (seq_t)1, (seq_t)5, (seq_t)10, (seq_t)25, (seq_t)50,
        (seq_t)100, (seq_t)200, (seq_t)500, (seq_t)1000, (seq_t)2000,
        (seq_t)5000, (seq_t)10000, (seq_t)50000, (seq_t)100000,
        (seq_t)500000, (seq_t)1000000, (seq_t)10000000
    };
    /* denom has 17 elements; a(n) is defined for 0 ≤ n ≤ 15 */
    if (n >= ((sizeof denom / sizeof denom[0]) - 1))
        return (seq_t)0;
    seq_t d_n    = denom[n];
    seq_t d_next = denom[n + 1];
    return (d_next - (seq_t)1) / d_n;
}
