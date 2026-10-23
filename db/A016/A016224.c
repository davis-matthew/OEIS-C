/*
 * A016224  Expansion of 1/((1-x)(1-4x)(1-8x)).
 * https://oeis.org/A016224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 13,
   a(n) = 12*a(n-1) - 32*a(n-2) + 1   for n ≥ 2   */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)13;

    seq_t a_nm2 = (seq_t)1;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)13;  /* a(n-1) */
    seq_t a_n   = (seq_t)0;

    for (idx_t i = 2; i <= n; ++i) {
        a_n = a_nm1 * ((seq_t)12) - a_nm2 * ((seq_t)32) + (seq_t)1;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
