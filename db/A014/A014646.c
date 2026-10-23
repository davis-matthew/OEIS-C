/*
 * A014646  Inverse of 637th cyclotomic polynomial.
 * https://oeis.org/A014646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 for n ≡ 0,7,14,21,28,35,42 (mod 637), a(n) = –1 for n ≡
 *            91,98,105,112,119,126 (mod 637), otherwise 0. Derived from the
 *            inverse power series of Φ₆₃₇(x)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* period of the sequence */
    const idx_t PERIOD = (idx_t)637;

    /* reduce the index modulo the period */
    idx_t idx = n % PERIOD;

    /* only multiples of 7 can be non‑zero */
    if (idx % (idx_t)7 != 0)
        return (seq_t)0;

    /* k = idx / 7   → k ∈ {0,…,90} */
    idx_t k = idx / (idx_t)7;

    if (k <= (idx_t)6)         /* 0,7,14,21,28,35,42  → 1 */
        return (seq_t)1;

    if (k >= (idx_t)13 && k <= (idx_t)18) /* 91,98,105,112,119,126 → –1 */
        return (seq_t)-1;

    /* all other multiples of 7 vanish */
    return (seq_t)0;
}
