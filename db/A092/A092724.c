/*
 * A092724  Ooguri-Vafa invariants of disk degeneracies for brane III in the
 *          O(K) -> P^1 x P^1 geometry.
 * https://oeis.org/A092724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, more
 * Formula:   a(n) = 0 for n <= 6; for n >= 7, let k = n - 6 and a(n) = -11 *
 *            (187*k^5 - 1518*k^4 + 4793*k^3 - 6390*k^2 + 2952*k)
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 11, i64 n <= 11, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 6) return 0;

    /* k = n - 6 (as seq_t to avoid unsigned underflow) */
    seq_t k = (seq_t)(n - 6);

    /* compute k powers */
    seq_t k2 = k * k;
    seq_t k3 = k2 * k;
    seq_t k4 = k3 * k;
    seq_t k5 = k4 * k;

    /* polynomial terms */
    seq_t t1 = (seq_t)187 * k5;
    seq_t t2 = (seq_t)1518 * k4;
    seq_t t3 = (seq_t)4793 * k3;
    seq_t t4 = (seq_t)6390 * k2;
    seq_t t5 = (seq_t)2952 * k;

    /* assemble polynomial */
    seq_t poly = t1 - t2 + t3 - t4 + t5;

    /* multiply by -11 */
    return (seq_t)(-11) * poly;
}
