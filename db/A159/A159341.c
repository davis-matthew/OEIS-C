/*
 * A159341  Transform of the finite sequence (1, 0, -1, 0, 1) by the T_{0,1}
 *          transformation (see link).
 * https://oeis.org/A159341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=2, a(1)=3, a(2)=6, a(3)=16, a(4)=39, a(5)=89, a(6)=206,
 *            a(n)=3*a(n-1)-2*a(n-2)+a(n-3) for n>=7
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial terms (offset 0). */
#ifdef __cplusplus
    static const seq_t init[7] = { seq_t(2), seq_t(3), seq_t(6),
                                   seq_t(16), seq_t(39), seq_t(89),
                                   seq_t(206) };
#else
    static const seq_t init[7] = { 2, 3, 6, 16, 39, 89, 206 };
#endif

    if (n < 7)                     /* return a known value directly */
        return init[n];

    /* Use a rolling buffer of the last three values. */
    seq_t v0 = init[4];            /* a(n-3) */
    seq_t v1 = init[5];            /* a(n-2) */
    seq_t v2 = init[6];            /* a(n-1) */

    for (idx_t i = 7; i <= n; ++i)
    {
        /* a(i) = 3*a(i-1) - 2*a(i-2) + a(i-3) */
        /* expressed with only + and - to avoid type‑dependent multiplication */
        seq_t next = v2 + v2 + v2   /* = 3*v2 */
                   - (v1 + v1)      /* = 2*v1 */
                   + v0;            /* + v0 */

        v0 = v1;
        v1 = v2;
        v2 = next;
    }

    return v2;
}
