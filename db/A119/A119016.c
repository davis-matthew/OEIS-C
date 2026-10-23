/*
 * A119016  Numerators of "Farey fraction" approximations to sqrt(2).
 * https://oeis.org/A119016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 51, u64 n <= 102,
 *            u128 n <= 203, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A119016 (numerators of Farey fraction approximations to sqrt(2))
 *
 * Recurrence:
 *   a(0) = 1,  a(1) = 0
 *   for n >= 2
 *       if n is even:  a(n) = a(n-1) + a(n-2)
 *       if n is odd :  a(n) = a(n-1) + a(n-3)
 */

seq_t a(idx_t n)
{
    /* handle base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* sliding window of the last three terms:
     *   a_nm3 = a(n-3), a_nm2 = a(n-2), a_nm1 = a(n-1)  */
    seq_t a_nm3 = (seq_t)1;  /* a(0) */
    seq_t a_nm2 = (seq_t)1;  /* a(0) is also a(n-2) at n = 2 */
    seq_t a_nm1 = (seq_t)0;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next;
        if ((i & 1) == 0) {          /* even i: use a(n-1) + a(n-2) */
            next = a_nm1 + a_nm2;
        } else {                     /* odd i:  use a(n-1) + a(n-3) */
            next = a_nm1 + a_nm3;
        }
        /* shift the window for the next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}
