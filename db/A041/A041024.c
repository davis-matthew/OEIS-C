/*
 * A041024  Numerators of continued fraction convergents to sqrt(17).
 * https://oeis.org/A041024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041024: numerators of convergents to sqrt(17).
   Formula:  a(0) = 4,  a(1) = 33,
             a(n) = 8*a(n-1) + a(n-2)  for n > 1                                  */

seq_t a(idx_t n)
{
    /* Handle the two initial values explicitly. */
    if (n == 0) return 4;
    if (n == 1) return 33;

    /* Iterate the linear recurrence up to n. */
    seq_t a0 = 4;          /* a(0) */
    seq_t a1 = 33;         /* a(1) */
    seq_t an;              /* holds a(k) for k >= 2 */

    for (idx_t k = 2; k <= n; ++k) {
        an = (seq_t)8 * a1 + a0; /* a(k) = 8*a(k-1) + a(k-2) */
        a0 = a1;
        a1 = an;
    }
    return a1;
}
