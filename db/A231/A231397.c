/*
 * A231397  Number of (1+1) X (n+1) 0..2 arrays with no element unequal to a
 *          strict majority of its horizontal, diagonal and antidiagonal
 *          neighbors, with values 0..2 introduced in row major order.
 * https://oeis.org/A231397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 4*a(n-1) + a(n-2) - 16*a(n-3) + 4*a(n-4) + 24*a(n-5) -
 *            16*a(n-6) * for n ≥ 7, * with a(1)=3, a(2)=7, a(3)=14, a(4)=33,
 *            a(5)=78, a(6)=189
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 1‑based index; values for n=1..6 are known constants */
    static const seq_t init[7] = {0, 3, 7, 14, 33, 78, 189};

    if (n <= 6) {
        return init[n];
    }

    /* iterate the recurrence from n=7 up to the requested n */
    seq_t a1 = init[1];   /* a(n-6) */
    seq_t a2 = init[2];   /* a(n-5) */
    seq_t a3 = init[3];   /* a(n-4) */
    seq_t a4 = init[4];   /* a(n-3) */
    seq_t a5 = init[5];   /* a(n-2) */
    seq_t a6 = init[6];   /* a(n-1) */

    for (idx_t k = 7; k <= n; ++k) {
        /* compute a(k) using the recurrence */
        seq_t pos = 4 * a6 + a5 + 4 * a3 + 24 * a2;
        seq_t neg = 16 * a4 + 16 * a1;
        seq_t next = pos - neg;

        /* shift the window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }

    return a6;
}
