/*
 * A145130  a(n) = 2 + (89040 + (71868 + (29932 + (8449 + (1960 + (322 + (28 +
 *          n)*n)*n)*n)*n)*n)*n)*n/40320.
 * https://oeis.org/A145130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + (89040 + (71868 + (29932 + (8449 + (1960 + (322 + (28
 *            + n)*n)*n)*n)*n)*n)*n)/40320
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 56, u64 n <= 960,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;                    /* cast index to seq_t */
    unsigned __int128 N128 = (unsigned __int128)N;  /* 128‑bit working register */

    /* Evaluate the polynomial t7 = n^8 + 28*n^7 + 322*n^6 + 1960*n^5
     *                  + 8449*n^4 + 29932*n^3 + 71868*n^2 + 89040*n
     */
    unsigned __int128 temp = N128;
    temp = (temp + 28) * N128;        /*  t1 = (28 + n) * n   */
    temp = (temp + 322) * N128;        /*  t2 = (322 + t1) * n */
    temp = (temp + 1960) * N128;       /*  t3 = (1960 + t2) * n */
    temp = (temp + 8449) * N128;       /*  t4 = (8449 + t3) * n */
    temp = (temp + 29932) * N128;      /*  t5 = (29932 + t4) * n */
    temp = (temp + 71868) * N128;      /*  t6 = (71868 + t5) * n */
    temp = (temp + 89040) * N128;      /*  t7 = (89040 + t6) * n */

    unsigned __int128 divided = temp / 40320;   /* t7 / 40320  */
    seq_t result = (seq_t)divided + 2;         /* final a(n)  */

    return result;
}
