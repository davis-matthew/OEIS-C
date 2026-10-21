/*
 * A221526  Number of 0..n arrays of length 6 with each element differing from
 *          at least one neighbor by 2 or more.
 * https://oeis.org/A221526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 - 20*n^4 + 83*n^3 - 182*n^2 + 236*n - 148 (for n > 3)
 *            Values for the offset: a(1) = 0 a(2) = 10 a(3) = 274
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)274;

    /* For n > 3 compute the polynomial using only unsigned arithmetic
       by grouping positive and negative parts separately.
       We avoid any intermediate negative values. */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;          // n^2
    seq_t n3 = n2 * sn;          // n^3
    seq_t n4 = n2 * n2;          // n^4
    seq_t n6 = n3 * n3;          // n^6  (or n4 * n2)

    seq_t pos = n6 + (seq_t)83 * n3 + (seq_t)236 * sn;          // n^6 + 83*n^3 + 236*n
    seq_t neg = (seq_t)20 * n4 + (seq_t)182 * n2 + (seq_t)148;  // 20*n^4 + 182*n^2 + 148

    return pos - neg;
}
