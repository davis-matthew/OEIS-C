/*
 * A018698  Divisors of 876.
 * https://oeis.org/A018698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 876 in ascending order. a(1)=1, a(2)=2, ..., a(12)=876. */
seq_t a(idx_t n)
{
    static const seq_t d[] = {1,2,3,4,6,12,73,146,219,292,438,876};
    if (n <= 12) return d[n-1];
    return 0;
}
