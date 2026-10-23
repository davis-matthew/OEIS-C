/*
 * A033131  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,1,0.
 * https://oeis.org/A033131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor((20/63)*4^n) Recurrence: a(n) = 4*a(n‑1) + a(n‑3) –
 *            4*a(n‑4) Initial values: a(1)=1, a(2)=5, a(3)=20, a(4)=81
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1 – behaviour undefined below */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)20;
    if (n == 4) return (seq_t)81;

    seq_t a_nm4 = (seq_t)1;  /* a(n-4) */
    seq_t a_nm3 = (seq_t)5;  /* a(n-3) */
    seq_t a_nm2 = (seq_t)20; /* a(n-2) */
    seq_t a_nm1 = (seq_t)81; /* a(n-1) */
    seq_t a_n   = 0;

    for (idx_t i = 5; i <= n; ++i) {
        a_n = (seq_t)4 * a_nm1 + a_nm3 - (seq_t)4 * a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
