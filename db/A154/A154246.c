/*
 * A154246  a(n) = ( (5 + sqrt(7))^n - (5 - sqrt(7))^n )/(2*sqrt(7)).
 * https://oeis.org/A154246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 18*a(n-2), with a(0)=0, a(1)=1 (offset 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* not used but harmless */
    seq_t a0 = 0;
    seq_t a1 = 1;
    if (n == 1)
        return a1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = ((seq_t)10) * a1 - ((seq_t)18) * a0;
        a0 = a1;
        a1 = next;
    }
    return a1;
}
