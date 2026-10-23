/*
 * A016036  Row sums of triangle A000369.
 * https://oeis.org/A016036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*(2*n-5)*a(n-1) - 3*(16*n^2-96*n+145)*a(n-2) * +
 *            2*(4*n-15)*(2*n-7)*(4*n-13)*a(n-3) + a(n-4) * Offset: a(1)=1
 *            (i.e. the index starts at 1) * Initial values: a(0)=1, a(1)=1,
 *            a(2)=4, a(3)=31
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Undefined for n == 0 (outside the OEIS offset) */
    if (n == 0) return (seq_t)0;

    /* Directly return the first four terms */
    if (n == 1) return (seq_t)1;      /* a(1) */
    if (n == 2) return (seq_t)4;      /* a(2) */
    if (n == 3) return (seq_t)31;     /* a(3) */

    /* Sliding‑window variables for a(n-4) … a(n-1) */
    seq_t a_nm4 = (seq_t)1;   /* a(0) */
    seq_t a_nm3 = (seq_t)1;   /* a(1) */
    seq_t a_nm2 = (seq_t)4;   /* a(2) */
    seq_t a_nm1 = (seq_t)31;  /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        /* Coefficient multiplications are performed in 64‑bit
           intermediate type and then cast to seq_t.  All products
           fit into 64 bits for the indices of interest (up to ~10⁴).  */
        seq_t term1 = a_nm1 * (seq_t)(6ULL * (2 * i - 5));
        seq_t term2 = a_nm2 * (seq_t)(3ULL * (16 * i * i - 96 * i + 145));
        seq_t term3 = a_nm3 * (seq_t)(2ULL * (4 * i - 15) *
                                      (2 * i - 7) *
                                      (4 * i - 13));
        seq_t cur = term1 + term3 + a_nm4 - term2;

        /* Shift the window for the next iteration */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return a_nm1;  /* a(n) */
}
