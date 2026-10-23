/*
 * A040877  Continued fraction for sqrt(908).
 * https://oeis.org/A040877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: continued fraction coefficients of sqrt(908).
   The coefficients are periodic after the first term: 30,
   followed by the repeating block [7,1,1,14,1,1,7,60]. */

seq_t a(idx_t n)
{
    /* Initial term */
    if (n == 0)
        return (seq_t)30;

    /* Periodic block after the initial term */
    static const seq_t period[8] = {
        (seq_t)7,
        (seq_t)1,
        (seq_t)1,
        (seq_t)14,
        (seq_t)1,
        (seq_t)1,
        (seq_t)7,
        (seq_t)60
    };

    /* (n-1) modulo 8 gives the correct index into the period */
    idx_t idx = (n - 1) % 8;
    return period[idx];
}
