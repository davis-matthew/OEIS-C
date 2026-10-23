/*
 * A040679  Continued fraction for sqrt(706).
 * https://oeis.org/A040679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = 26, 1, 1, 3, 26, 3, 1, 1, 52 repeating thereafter with
 *            offset 0, so a(0)=26 and for n>=1 a(n)=pattern[(n-1) mod 8]
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)26u;

    /* periodic part after a(0) */
    static const seq_t pat[8] = {
        (seq_t)1u,   /* a(1) */
        (seq_t)1u,   /* a(2) */
        (seq_t)3u,   /* a(3) */
        (seq_t)26u,  /* a(4) */
        (seq_t)3u,   /* a(5) */
        (seq_t)1u,   /* a(6) */
        (seq_t)1u,   /* a(7) */
        (seq_t)52u   /* a(8) */
    };

    /* Since idx_t is unsigned, subtraction and modulo are safe */
    return pat[(n - 1u) % 8u];
}
