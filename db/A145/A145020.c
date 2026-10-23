/*
 * A145020  a(n) = ((7 + sqrt(7))^n - (7 - sqrt(7))^n)^2/28.
 * https://oeis.org/A145020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=196, a(3)=23716 ;
 *            a(n)=154*a(n-1)-6468*a(n-2)+74088*a(n-3) for n>=4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If called with an index below the offset, return 0 (undefined behaviour otherwise). */
    if (n < 1) return (seq_t)0;

    /* Base cases */
    seq_t a1 = (seq_t)1;
    if (n == 1) return a1;
    seq_t a2 = (seq_t)196;
    if (n == 2) return a2;
    seq_t a3 = (seq_t)23716;
    if (n == 3) return a3;

    /* Iterate using the recurrence. */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)154 * a3
                    - (seq_t)6468 * a2
                    + (seq_t)74088 * a1;
        a1 = a2;  /* shift: a(n-3) becomes a(n-2)   */
        a2 = a3;  /* shift: a(n-2) becomes a(n-1)   */
        a3 = next;/* shift: a(n-1) becomes a(n)    */
    }

    return a3;
}
