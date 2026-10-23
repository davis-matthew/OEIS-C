/*
 * A032126  "BIK" (reversible, indistinct, unlabeled) transform of 1,2,3,4...
 * https://oeis.org/A032126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 3*a(n-1)+2*a(n-2)-9*a(n-3)+2*a(n-4)+3*a(n-5)-a(n-6) for n>=6
   Initial values: a(1)=1, a(2)=3, a(3)=6, a(4)=15, a(5)=33
   This implementation uses the recurrence derived from the generating
   function; it works for any seq_t type supported by the build system. */

seq_t a(idx_t n)
{
    /* Base cases */
    static const seq_t base[5] = {(seq_t)1, (seq_t)3, (seq_t)6,
                                  (seq_t)15, (seq_t)33};
    if (n == 0) return 0;          /* offset is 1 – undefined for n==0 */
    if (n <= 5) return base[n - 1];

    /* Use a moving window of the six previous values */
    seq_t vals[6];
    vals[0] = 0;                    /* a(0) */
    vals[1] = base[0];              /* a(1) */
    vals[2] = base[1];              /* a(2) */
    vals[3] = base[2];              /* a(3) */
    vals[4] = base[3];              /* a(4) */
    vals[5] = base[4];              /* a(5) */

    seq_t result = 0;
    for (idx_t k = 6; k <= n; ++k) {
        /* Combination of positive terms first to avoid wrap‑around */
        seq_t pos = vals[1] * (seq_t)3;
        pos += vals[2] * (seq_t)2;
        pos += vals[4] * (seq_t)2;
        pos += vals[5] * (seq_t)3;

        seq_t sub = vals[0] + vals[3] * (seq_t)9;

        result = pos - sub;        /* pos >= sub for all n >= 6 */

        /* Shift the window for next iteration */
        vals[0] = vals[1];
        vals[1] = vals[2];
        vals[2] = vals[3];
        vals[3] = vals[4];
        vals[4] = vals[5];
        vals[5] = result;
    }

    return result;
}
