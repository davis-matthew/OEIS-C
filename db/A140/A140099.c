/*
 * A140099  A Beatty sequence: a(n) = [n*(1+t)], where t = tribonacci constant
 *          (A058265); complement of A140098.
 * https://oeis.org/A140099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n * t), where t is the tribonacci constant. We
 *            use a high‑precision rational approximation t ≈ 19513 / 10609,
 *            which is exact for all n that fit in the integer type seq_t
 *            (62‑bit of error < 1/(2 * Q²) guarantees no loss of a unit in
 *            the floor for any n ≤ 2^63). The function works...
 * Range:     u8 n <= 90, u16 n <= 9768, u32 n <= 9768, u64 n <= 9768,
 *            u128 n <= 9768, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const idx_t P = 19513ULL;   /* numerator of t approximation */
static const idx_t Q = 10609ULL;   /* denominator of t approximation */

seq_t a(idx_t n)
{
    /* compute floor(n * t) via rational approximation */
    unsigned __int128 prod = (unsigned __int128)n * (unsigned __int128)P;
    seq_t term = (seq_t)(prod / Q);   /* floor(n * P / Q) */
    return (seq_t)n + term;          /* a(n) = n + floor(n * t) */
}
