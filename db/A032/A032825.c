/*
 * A032825  Numbers whose set of base-13 digits is {1,4}.
 * https://oeis.org/A032825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1, a(2)=4; a(n)=13*a(floor(n/2))+1 for n odd,
 *            a(n)=13*a(floor((n-1)/2))+4 for n even
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) {
        return n == 1 ? (seq_t)1 : (seq_t)4;
    }
    if (n & 1) { /* odd */
        return (seq_t)13 * a(n / 2) + (seq_t)1;
    } else {    /* even */
        return (seq_t)13 * a((n - 1) / 2) + (seq_t)4;
    }
}
