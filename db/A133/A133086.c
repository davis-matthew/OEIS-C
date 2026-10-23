/*
 * A133086  Row sums of triangle A133085.
 * https://oeis.org/A133086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A133086 – Row sums of triangle A133085
   Formula (for n > 1):  a(n) = 2^n + 3*n*2^(n‑2)
   Recurrence (n ≥ 4):  a(n) = 4*(a(n‑1) – a(n‑2))
   Initial values:  a(0)=1, a(1)=4, a(2)=10, a(3)=26
   This implementation uses the recurrence, which is fast and
   works for any integer type defined as seq_t. */
seq_t a(idx_t n)
{
    /* Handle the trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)26;

    /* a_nm2  = a(n‑2), a_nm1 = a(n‑1)  */
    seq_t a_nm2 = (seq_t)10;    /* a(2) */
    seq_t a_nm1 = (seq_t)26;    /* a(3) */
    seq_t a_n   = a_nm1;        /* temporary holder */

    for (idx_t i = 4; i <= n; ++i) {
        a_n = (seq_t)4 * (a_nm1 - a_nm2); /* recurrence */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
