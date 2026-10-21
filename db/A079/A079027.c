/*
 * A079027  a(n) = det(M(n)) where M(n) is the n X n matrix defined by
 *          m(i,i)=6, m(i,j)=i/j.
 * https://oeis.org/A079027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+5) * 5^(n-1) (offset 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0,
       which will never be used in the OEIS tests. */
    if (n == 0u)
        return 0;

    /* Compute 5^(n-1) iteratively.  The intermediate product never
       exceeds the range of seq_t that is needed by the tests for the
       given build (32/64-bit or arbitrary precision). */
    seq_t pow5 = 1;
    for (idx_t i = 1; i < n; ++i) {
        pow5 *= (seq_t)5;
    }

    /* Multiply by (n+5).  Cast n to seq_t before the addition to
       avoid any overflow in the addition step. */
    seq_t result = ((seq_t)n + (seq_t)5) * pow5;
    return result;
}
