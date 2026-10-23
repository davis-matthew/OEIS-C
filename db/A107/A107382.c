/*
 * A107382  a(n) = 4*a(n-1)-4*a(n-3)-a(n-4).
 * https://oeis.org/A107382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)-4*a(n-3)-a(n-4); a(0)=4, a(1)=4, a(2)=16,
 *            a(3)=52
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 4;
    if (n == 1) return 4;
    if (n == 2) return 16;
    if (n == 3) return 52;

    /* Keep the last four values: a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a_nm4 = 4;  /* a(0) */
    seq_t a_nm3 = 4;  /* a(1) */
    seq_t a_nm2 = 16; /* a(2) */
    seq_t a_nm1 = 52; /* a(3) */
    seq_t cur   = 0;  /* a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        /* use 128‑bit arithmetic to avoid overflow in intermediate steps */
        unsigned __int128 tmp = (unsigned __int128)a_nm1 * 4;
        tmp -= (unsigned __int128)a_nm3 * 4;
        tmp -= (unsigned __int128)a_nm4;
        cur = (seq_t)tmp;

        /* shift window */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return cur;
}
