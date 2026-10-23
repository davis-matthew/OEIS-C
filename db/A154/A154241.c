/*
 * A154241  a(n) = ( (9 + sqrt(6))^n - (9 - sqrt(6))^n )/(2*sqrt(6)).
 * https://oeis.org/A154241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(n)=18*a(n-1)-75*a(n-2) for n>1 */

/* Computes the nth term of OEIS A154241.
   Indexing follows OEIS offset: a(1) is the first term. */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined in OEIS, return 0 for safety */
        return 0;
    if (n == 1)
        return 1;

    seq_t a0 = 0;              /* a(0) */
    seq_t a1 = 1;              /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = 18 * a1 - 75 * a0;
        a0 = a1;
        a1 = next;
    }
    return a1;
}
