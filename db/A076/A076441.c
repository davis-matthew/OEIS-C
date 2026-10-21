/*
 * A076441  Let u(1) = u(2) = u(3) = 1; u(n) = sign(u(n-1)-u(n-2))*u(n-3),
 *          then a(n) = 1+u(n).
 * https://oeis.org/A076441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   u(1)=u(2)=u(3)=1; u(n)=sign(u(n-1)-u(n-2))*u(n-3); a(n)=1+u(n)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* First three terms are constant. */
    if (n <= 3U)
        return (seq_t)2;

    /* 9‑periodic pattern for u(n) starting at n = 4. */
    static const int8_t u_pattern[9] = {
        0,  /* u(4) */
       -1, /* u(5) */
       -1, /* u(6) */
        0, /* u(7) */
       -1, /* u(8) */
        1, /* u(9) */
        0, /* u(10) */
        1, /* u(11) */
        1  /* u(12) */
    };

    /* n ≥ 4, so (n-4) fits in idx_t. */
    idx_t idx = (n - 4U) % 9U;
    int8_t u_val = u_pattern[idx];

    return (seq_t)(u_val + 1);   /* a(n) = 1 + u(n) */
}
