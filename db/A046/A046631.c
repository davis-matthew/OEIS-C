/*
 * A046631  Number of cubic residues mod 3^n.
 * https://oeis.org/A046631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=3, a(3)=7; for n≥4 a(n) = 3*a(n‑1) + a(n‑3)
 *            – 3*a(n‑4)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)7;

    /* Initialise the last four values used in the recurrence */
    seq_t a_nm4 = (seq_t)1;  /* a(n-4) */
    seq_t a_nm3 = (seq_t)3;  /* a(n-3) */
    seq_t a_nm2 = (seq_t)3;  /* a(n-2) */
    seq_t a_nm1 = (seq_t)7;  /* a(n-1) */
    seq_t a_n   = (seq_t)0;  /* current a(n) */

    /* Compute the sequence up to index n */
    for (idx_t i = 4; i <= n; ++i) {
        a_n = a_nm1;                /* a(n-1) */
        a_n = a_n * (seq_t)3;       /* 3*a(n-1) */
        a_n += a_nm3;               /* + a(n-3) */
        a_n -= a_nm4 * (seq_t)3;    /* - 3*a(n-4) */

        /* Shift the window of previous values */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
