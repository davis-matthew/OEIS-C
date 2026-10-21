/*
 * A232937  Number of n X 4 0..2 arrays with no element x(i,j) adjacent to
 *          value 2-x(i,j) horizontally, vertically or antidiagonally.
 * https://oeis.org/A232937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 10*a(n-2) + 15*a(n-3) + 4*a(n-4) - 6*a(n-5) -
 *            a(n-6) + 3*a(n-7) - a(n-8) Initial values (n=1..9): 24, 56, 252,
 *            1048, 4458, 18850, 79810, 337850, 1430144 (values taken from the
 *            OEIS entry)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values a(1) .. a(9) */
    static const seq_t base[9] = {
        (seq_t)24, (seq_t)56, (seq_t)252, (seq_t)1048,
        (seq_t)4458, (seq_t)18850, (seq_t)79810,
        (seq_t)337850, (seq_t)1430144
    };

    if (n == 0)            /* out of range, behaviour defined as 0 */
        return (seq_t)0;
    if (n <= 9)
        return base[n - 1];

    /* Buffer holding the 8 most recent terms:
       buf[0] = a(k-1), buf[1] = a(k-2), ..., buf[7] = a(k-8)   */
    seq_t buf[8];
    for (int i = 0; i < 8; ++i)
        buf[i] = base[8 - i];          /* start with a(9)..a(2) */

    seq_t result, sum_pos, sum_neg;

    for (idx_t k = 10; k <= n; ++k) {
        /* Positive part of the recurrence */
        sum_pos = buf[0];
        sum_pos += (seq_t)10 * buf[1];
        sum_pos += (seq_t)15 * buf[2];
        sum_pos += (seq_t)4  * buf[3];
        sum_pos += (seq_t)3  * buf[6];

        /* Negative part of the recurrence */
        sum_neg = (seq_t)6 * buf[4];
        sum_neg += buf[5];
        sum_neg += buf[7];

        result = sum_pos - sum_neg;     /* guaranteed non‑negative */

        /* Shift buffer right: drop oldest term and insert new one */
        for (int i = 7; i >= 1; --i)
            buf[i] = buf[i - 1];
        buf[0] = result;
    }

    return buf[0];   /* a(n) */
}
