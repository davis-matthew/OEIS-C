/*
 * A042119  Denominators of continued fraction convergents to sqrt(584).
 * https://oeis.org/A042119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1, a(1)=6, a(2)=289, a(3)=1740, a(n)=290*a(n-2)-a(n-4) for
 *            n>3 (continued fraction convergents of sqrt(584))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)289;
    if (n == 3) return (seq_t)1740;

    /* The recurrence depends on a(n‑2) and a(n‑4), so keep the
       last four computed values. */
    seq_t a_nm4 = (seq_t)1;      /* a(n-4) */
    seq_t a_nm3 = (seq_t)6;      /* a(n-3) */
    seq_t a_nm2 = (seq_t)289;    /* a(n-2) */
    seq_t a_nm1 = (seq_t)1740;   /* a(n-1) */

    seq_t a_n;                   /* a(n) to be computed */

    for (idx_t i = 4; i <= n; ++i) {
        a_n = (seq_t)290 * a_nm2 - a_nm4;

        /* Shift the window: a(n-4) becomes a(n-3), etc. */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}
