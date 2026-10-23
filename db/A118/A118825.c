/*
 * A118825  Numerators of the convergents of the 2-adic continued fraction of
 *          zero given by A118824.
 * https://oeis.org/A118825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, sign, easy
 * Formula:   a(n) = sqrt((n+1)^2 mod 8)*(-1)^floor((n+3)/4)
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Equivalently: a(n) repeats the period 8 sequence */
/* [-2,-1,0,-1,2,1,0,1]) */
seq_t a(idx_t n)
{
    /* periodic pattern starting at n = 1 */
    static const seq_t pat[8] = {
        (seq_t)-2, (seq_t)-1, (seq_t)0, (seq_t)-1,
        (seq_t)2,  (seq_t)1,  (seq_t)0, (seq_t)1
    };
    /* n >= 1: use (n-1) modulo 8 (8 is a power of two) */
    return pat[((n - 1) & 7)];
}
