/*
 * A167910  a(n) = (4*3^n - 5*2^n + (-2)^n)/20.
 * https://oeis.org/A167910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) + 4*a(n-2) - 12*a(n-3), a(0)=0, a(1)=0, a(2)=1 */
seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n < 3) {
        return (n == 2) ? (seq_t)1 : (seq_t)0;
    }

    /* Use a three‑variable rolling computation to avoid any large buffers. */
    seq_t a_nm3 = (seq_t)0;   /* a(n-3)  */
    seq_t a_nm2 = (seq_t)0;   /* a(n-2)  */
    seq_t a_nm1 = (seq_t)1;   /* a(n-1)  */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t val = 3 * a_nm1 + 4 * a_nm2 - 12 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = val;
    }

    return a_nm1;
}
