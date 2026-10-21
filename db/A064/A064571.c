/*
 * A064571  Binomial transform of (3n)!.
 * https://oeis.org/A064571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binom(n,k)*(3k)! Recurrence (valid for all n
 *            ≥ 6): a(n) = (27n³-27n²+6n+1)a(n‑1) - 3(n-1)(27n²-45n+20)a(n‑2)
 *            + 27(n-2)(n-1)(3n-4)a(n‑3) - 27(n-3)(n-2)(n-1)a(n‑4) Base
 *            values: a(0)=1, a(1)=7, a(2)=733, a(3)=365059, a(4)=480457465,
 *            a(5)=1310073012031
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 7;
    if (n == 2) return 733;
    if (n == 3) return 365059;
    if (n == 4) return 480457465;
    if (n == 5) return 1310073012031;

    /* Variables holding a(n-4), a(n-3), a(n-2), a(n-1) for the iteration */
    seq_t a_nm4 = 733;                  /* a(2) */
    seq_t a_nm3 = 365059;               /* a(3) */
    seq_t a_nm2 = 480457465;            /* a(4) */
    seq_t a_nm1 = 1310073012031;        /* a(5) */
    seq_t next = 0;

    for (idx_t i = 6; i <= n; ++i) {
        seq_t ns = (seq_t)i;

        seq_t c1 = ((seq_t)27) * ns * ns * ns
                 - ((seq_t)27) * ns * ns
                 + ((seq_t)6) * ns
                 + ((seq_t)1);

        seq_t c2 = ((seq_t)3) * (ns - 1) *
                 (((seq_t)27) * ns * ns
                 - ((seq_t)45) * ns
                 + ((seq_t)20));

        seq_t c3 = ((seq_t)27) * (ns - 2) * (ns - 1) * ( ((seq_t)3) * ns - ((seq_t)4) );

        seq_t c4 = ((seq_t)27) * (ns - 3) * (ns - 2) * (ns - 1);

        next = c1 * a_nm1 - c2 * a_nm2 + c3 * a_nm3 - c4 * a_nm4;

        /* shift for next iteration */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return next;
}
