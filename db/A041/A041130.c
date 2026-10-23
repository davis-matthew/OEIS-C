/*
 * A041130  Numerators of continued fraction convergents to sqrt(74).
 * https://oeis.org/A041130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of convergents to sqrt(74).  Recurrence:
   a(0) = 8
   a(1) = 9
   for n >= 2, let coeff(n) = 16 if n % 5 == 0 else 1,
   then a(n) = coeff(n) * a(n-1) + a(n-2). */
seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == 0) return (seq_t)8;
    if (n == 1) return (seq_t)9;

    seq_t pprev = (seq_t)8;   /* a(n-2) */
    seq_t pcur  = (seq_t)9;   /* a(n-1) */
    seq_t anum;

    for (idx_t k = 2; k <= n; ++k) {
        anum = (k % 5 == 0 ? (seq_t)16 : (seq_t)1);
        anum = anum * pcur + pprev;   /* a(n) = coeff * a(n-1) + a(n-2) */
        pprev = pcur;
        pcur  = anum;
    }

    return pcur;
}
