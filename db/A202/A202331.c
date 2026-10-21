/*
 * A202331  Number of (n+1) X 5 binary arrays with consecutive windows of two
 *          bits considered as a binary number nondecreasing in every row and
 *          column.
 * https://oeis.org/A202331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6) (n>6) with a(1)=49, a(2)=129, a(3)=289, a(4)=576,
 *            a(5)=1052, a(6)=1796
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 187, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle calls below the offset */
    if (n <= 0)
        return 0;

    /* base values for a(1) .. a(6) */
    static const seq_t base[6] = {49, 129, 289, 576, 1052, 1796};

    if (n <= 6)
        return base[n - 1];

    /* temporary storage for the last six values of the recurrence */
    seq_t v[6];
    v[0] = 49;
    v[1] = 129;
    v[2] = 289;
    v[3] = 576;
    v[4] = 1052;
    v[5] = 1796;

    for (idx_t i = 7; i <= n; ++i)
    {
        /* compute the next term using the recurrence */
        seq_t next = (seq_t)6 * v[5]
                   - (seq_t)15 * v[4]
                   + (seq_t)20 * v[3]
                   - (seq_t)15 * v[2]
                   + (seq_t)6 * v[1]
                   - v[0];
        /* shift buffer */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = next;
    }
    return v[5];
}
