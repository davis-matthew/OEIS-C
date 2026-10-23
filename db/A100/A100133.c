/*
 * A100133  a(n) = Sum_{k=0..floor(n/4)} C(n-2k,2k) * 3^k * 2^(n-4k).
 * https://oeis.org/A100133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/4⌋} C(n-2k,2k) * 3^k * 2^(n-4k) Recurrence:
 *            a(0)=1, a(1)=2, a(2)=4, a(3)=8; a(n) = 4*a(n-1) - 4*a(n-2) +
 *            3*a(n-4) (n ≥ 4)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)8;

    /* iterate recurrence */
    seq_t a_nm4 = (seq_t)1; /* a(n-4) */
    seq_t a_nm3 = (seq_t)2; /* a(n-3) */
    seq_t a_nm2 = (seq_t)4; /* a(n-2) */
    seq_t a_nm1 = (seq_t)8; /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (a_nm1 << 2) - (a_nm2 << 2) + (a_nm4 * (seq_t)3);
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}
