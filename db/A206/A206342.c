/*
 * A206342  Number of (n+1)X8 0..3 arrays with every 2X2 subblock having
 *          nonzero determinant and commuting with every horizontal or
 *          vertical neighbor.
 * https://oeis.org/A206342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 3*a(n-3) for n>16 Initial terms:
 *            a(1)=4111, a(2)=2042, a(3)=12005, a(4)=6078, a(5)=36015,
 *            a(6)=18258, a(7)=108141, a(8)=54762, a(9)=324087, a(10)=163950,
 *            a(11)=971925, a(12)=491514, a(13)=2915439, a(14)=1474206,
 *            a(15)=8746143, a(16)=4422606
 * Range:     u8 none, u16 n <= 6, u32 n <= 26, u64 n <= 66, u128 n <= 148,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t init_vals[16] = {
    4111,
    2042,
    12005,
    6078,
    36015,
    18258,
    108141,
    54762,
    324087,
    163950,
    971925,
    491514,
    2915439,
    1474206,
    8746143,
    4422606
};

seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for n<1; return 0 as a safe default */
        return 0;

    if (n <= 16)
        return init_vals[n - 1];

    /* For n > 16 compute iteratively using the recurrence.
       Only three most recent values are needed. */
    seq_t a1 = init_vals[13]; /* a(14) */
    seq_t a2 = init_vals[14]; /* a(15) */
    seq_t a3 = init_vals[15]; /* a(16) */

    for (idx_t i = 17; i <= n; ++i) {
        seq_t next = a3 + 3 * a2 - 3 * a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
