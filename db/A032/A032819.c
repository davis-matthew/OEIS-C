/*
 * A032819  Numbers whose set of base-7 digits is {1,4}.
 * https://oeis.org/A032819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1, a(2)=4; * a(n) = 7*a(floor(n/2)) + 1 for odd n * a(n) =
 *            7*a((n-1)/2) + 4 for even n * Offset: 1 (a(1)=1)
 * Range:     u8 n <= 14, u16 n <= 94, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t _a_rec(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)4;
    if (n & (idx_t)1)   /* odd */
        return ((seq_t)7) * _a_rec(n >> (idx_t)1) + (seq_t)1;
    else                /* even */
        return ((seq_t)7) * _a_rec((n >> (idx_t)1) - (idx_t)1) + (seq_t)4;
}

seq_t a(idx_t n)
{
    return _a_rec(n);
}
