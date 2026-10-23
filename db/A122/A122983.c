/*
 * A122983  a(n) = (2 + (-1)^n + 3^n)/4.
 * https://oeis.org/A122983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2 + (-1)^n + 3^n)/4
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is implemented as                          */
/* a(n) = floor(3^n / 4) + 1, which is equivalent for all n */
/* using only operations allowed on seq_t.                 */

seq_t a(idx_t n)
{
    /* floor(3^n / 4) will be accumulated in q  */
    seq_t q = 0;
    /* remainder of 3^k modulo 4 (0..3) */
    uint8_t r = 1;   /* 3^0 % 4 = 1 */

    /* iterate n times: multiply by 3 and update q and r   */
    for (idx_t i = 0; i < n; ++i)
    {
        /* old remainder before the multiplication */
        uint8_t old_r = r;

        /* 3 * old_r fits in an 8‑bit integer (max 9) */
        uint8_t mul = 3 * old_r;          /* 0 <= mul <= 9 */

        /* new remainder: 3 * old_r mod 4 */
        r = mul & 3;          /* mul % 4 */

        /* contribution of the high part: (3 * old_r) / 4   */
        seq_t add = mul >> 2; /* mul / 4 (integer division) */

        /* update the quotient: q = 3*q + add                */
        q = 3 * q + add;
    }

    /* final result: floor(3^n / 4) + 1 */
    return q + 1;
}
