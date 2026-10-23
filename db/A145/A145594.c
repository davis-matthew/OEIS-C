/*
 * A145594  a(n) = A145593(n) mod 9.
 * https://oeis.org/A145594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = decimal digit of (92696389/111111111) after the decimal
 *            point, modulo 9. The decimal expansion of the fraction is purely
 *            periodic with period 9, so the sequence repeats a(0)…a(8). The
 *            digits are generated once and stored in a static array
 * Range:     u8 none, u16 none, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* static cache of the 9-period digits */
    static int initialized = 0;
    static seq_t period[9];

    if (!initialized) {
        const seq_t num = (seq_t)92696389;
        const seq_t den = (seq_t)111111111;
        seq_t rem = num % den;          /* initial remainder */
        for (idx_t i = 0; i < 9; ++i) {
            rem *= (seq_t)10;           /* bring next digit into place */
            seq_t digit = rem / den;    /* next decimal digit */
            rem = rem % den;            /* new remainder for next step */
            period[i] = digit % (seq_t)9; /* mod 9 (9 becomes 0) */
        }
        initialized = 1;
    }
    idx_t idx = n % (idx_t)9;           /* period index */
    return period[idx];
}
