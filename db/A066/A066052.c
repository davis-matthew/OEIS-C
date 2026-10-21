/*
 * A066052  Number of permutations in the symmetric group S_n with order >= 3.
 * https://oeis.org/A066052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! - A000085(n)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    seq_t fact = 1;
    seq_t inv0 = 1;   /* I(0) */
    seq_t inv1 = 1;   /* I(1) */
    seq_t invn = 0;

    if (n >= 2) {
        for (idx_t i = 2; i <= n; ++i) {
            fact *= (seq_t)i;                     /* compute i! for factorial */
            invn = inv1 + (seq_t)(i - 1) * inv0;  /* I(i) = I(i-1) + (i-1)*I(i-2) */
            inv0 = inv1;
            inv1 = invn;
        }
    }

    seq_t invol;
    if (n == 0) {
        invol = inv0;
    } else if (n == 1) {
        invol = inv1;
    } else {
        invol = inv1;   /* inv1 holds I(n) after loop */
    }

    /* factorial for n==1 was already 1, for n>=2 computed in loop */
    return fact - invol;
}
