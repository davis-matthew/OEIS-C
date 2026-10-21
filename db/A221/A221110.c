/*
 * A221110  Number of n X 4 arrays of occupancy after each element moves to
 *          some horizontal or antidiagonal neighbor, without 2-loops.
 * https://oeis.org/A221110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 29*a(n-1) - 210*a(n-2) + 451*a(n-3) - 288*a(n-4) +
 *            20*a(n-5) - a(n-6) Initial values: a(1)=0, a(2)=7, a(3)=178,
 *            a(4)=3706, a(5)=73267, a(6)=1424745
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first six terms directly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)178;
    if (n == 4) return (seq_t)3706;
    if (n == 5) return (seq_t)73267;
    if (n == 6) return (seq_t)1424745;

    /* Roll‑through variables holding the last six computed values. */
    seq_t a1 = (seq_t)0;        /* a(n-6)   */
    seq_t a2 = (seq_t)7;        /* a(n-5)   */
    seq_t a3 = (seq_t)178;      /* a(n-4)   */
    seq_t a4 = (seq_t)3706;     /* a(n-3)   */
    seq_t a5 = (seq_t)73267;    /* a(n-2)   */
    seq_t a6 = (seq_t)1424745;  /* a(n-1)   */

    /* Compute successive terms up to n. */
    for (idx_t i = 7; i <= n; ++i) {
        /* Apply the recurrence. */
        seq_t new_val = (seq_t)29 * a6
                        - (seq_t)210 * a5
                        + (seq_t)451 * a4
                        - (seq_t)288 * a3
                        + (seq_t)20  * a2
                        - a1;

        /* Shift the window forward. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = new_val;
    }
    return a6;
}
