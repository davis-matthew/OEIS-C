/*
 * A021024  Expansion of g.f. 1/((1-x)*(1-2*x)*(1-3*x)*(1-5*x)).
 * https://oeis.org/A021024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) – 41*a(n-2) + 61*a(n-3) – 30*a(n-4) with the
 *            initial values a(0)=1, a(1)=11, a(2)=80, a(3)=490. The
 *            recurrence is used for all n ≥ 4. The values are exact for any
 *            seq_t capable of arbitrary precision or for the 64‑bit build as
 *            long as the result fits in uint64_t
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)80;
    if (n == 3) return (seq_t)490;

    /* Store the four preceding terms. */
    seq_t a_nm4 = (seq_t)1;     /* a(n-4) */
    seq_t a_nm3 = (seq_t)11;    /* a(n-3) */
    seq_t a_nm2 = (seq_t)80;    /* a(n-2) */
    seq_t a_nm1 = (seq_t)490;   /* a(n-1) */
    seq_t curr;

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute the next term using the recurrence.  The
           sum of the positive contributions dominates for n ≥ 4,
           so the subtraction cannot underflow.                 */
        seq_t sum_pos = (seq_t)11 * a_nm1 + (seq_t)61 * a_nm3;
        seq_t sum_neg = (seq_t)41 * a_nm2 + (seq_t)30 * a_nm4;
        curr = sum_pos - sum_neg;

        /* Slide the window of preceding terms. */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = curr;
    }

    return a_nm1;
}
