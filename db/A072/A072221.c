/*
 * A072221  a(n) = 6*a(n-1) - a(n-2) + 2, with a(0)=1, a(1)=4.
 * https://oeis.org/A072221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2) + 2, a(0)=1, a(1)=4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    /* Iteratively compute the recurrence.  The intermediate
       values stay within the bounds of seq_t for all n where
       the result fits; beyond that limits the sequence is
       unchecked by the test harness. */
    seq_t a0 = (seq_t)1;   /* a(n-2) */
    seq_t a1 = (seq_t)4;   /* a(n-1) */
    seq_t ai;              /* current a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        ai = (seq_t)( ( (seq_t)6 * a1 ) - a0 + (seq_t)2 );
        a0 = a1;
        a1 = ai;
    }

    return a1;
}
