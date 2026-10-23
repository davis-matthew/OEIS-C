/*
 * A174784  Expansion of x*(1-x+x^3+x^4)/(1+x^6) (Period 12).
 * https://oeis.org/A174784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = periodic sequence of period 12: a(n) = [0, 1, ‑1, 0, 1,
 *            1, 0, ‑1, 1, 0, ‑1, ‑1]_{r = n mod 12} The sequence repeats
 *            every 12 terms
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t seq[12] = {
        (seq_t)0,  (seq_t)1,  (seq_t)-1, (seq_t)0,
        (seq_t)1,  (seq_t)1,  (seq_t)0,  (seq_t)-1,
        (seq_t)1,  (seq_t)0,  (seq_t)-1, (seq_t)-1
    };
    return seq[n % 12];
}
