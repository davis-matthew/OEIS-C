/*
 * A032804  Numbers whose set of base-4 digits is {2,3}.
 * https://oeis.org/A032804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 0, a(2*n) = 4*a(n-1) + 3, a(2*n+1) = 4*a(n) + 2
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS entry A032804 */

static seq_t _a_rec(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    if ((n & 1) == 0) {           /* even: n = 2*m */
        idx_t m = n >> 1;
        return ((seq_t)4 * _a_rec(m - 1)) + 3;
    } else {                      /* odd: n = 2*m + 1 */
        idx_t m = n >> 1;
        return ((seq_t)4 * _a_rec(m)) + 2;
    }
}

seq_t a(idx_t n)
{
    return _a_rec(n);
}
