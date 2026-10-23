/*
 * A041314  Numerators of continued fraction convergents to sqrt(171).
 * https://oeis.org/A041314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numerators of continued fraction convergents to sqrt(171).
   Recurrence:
     a(0) = 13
     a(1) = 170
     For n >= 2, let b(n) = 13 if n is odd, 26 if n is even.
     Then a(n) = b(n) * a(n-1) + a(n-2).                            */
seq_t a(idx_t n)
{
    seq_t p_nm2 = 0;   /* p_{-2} */
    seq_t p_nm1 = 1;   /* p_{-1} */
    seq_t p = 0;
    for (idx_t i = 0; i <= n; ++i) {
        seq_t b = (i == 0) ? 13 : ((i & 1) ? 13 : 26);
        p = b * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = p;
    }
    return p;
}
