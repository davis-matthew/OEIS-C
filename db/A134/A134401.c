/*
 * A134401  Row sums of triangle A134400.
 * https://oeis.org/A134401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*2^n for n≥1, a(0) = 1 Recurrence: a(0)=1, a(1)=2,
 *            a(2)=8 a(n) = 4*a(n-1) - 4*a(n-2) for n>2 The recurrence uses
 *            only shifts and subtraction, so it works for unsigned 8‑, 16‑,
 *            32‑, 64‑, 128‑bit integers as well as for arbitrary‑precision
 *            types
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)8;

    seq_t a_nm2 = (seq_t)2;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)8;   /* a(n-1) */
    seq_t a_n   = 0;

    for (idx_t i = 3; i <= n; ++i) {
        a_n = (a_nm1 << 2) - (a_nm2 << 2); /* 4*a(n-1) - 4*a(n-2) */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
