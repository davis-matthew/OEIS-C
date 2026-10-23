/*
 * A041714  Numerators of continued fraction convergents to sqrt(377).
 * https://oeis.org/A041714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A041714: Numerators of continued fraction convergents to sqrt(377).
 * Recurrence: a(0)=19, a(1)=39,
 * for n>=2: a(n) = f(n)*a(n-1) + a(n-2),
 * where f(n) = 38 if n%4==0 else 2.
 * The coefficients arise from the periodic continued fraction [19; overline{2,2,2,38}].
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)19;
    if (n == 1) return (seq_t)39;

    seq_t p0 = (seq_t)19;   /* a(n-2) */
    seq_t p1 = (seq_t)39;   /* a(n-1) */
    seq_t p_next;

    for (idx_t k = 2; k <= n; ++k) {
        seq_t factor = (k % 4 == 0 ? (seq_t)38 : (seq_t)2);
        p_next = factor * p1 + p0;
        p0 = p1;
        p1 = p_next;
    }
    return p1;
}
