/*
 * A016288  Inverse of 2279th cyclotomic polynomial.
 * https://oeis.org/A016288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 + ⌊n/2279⌋ – ( ⌊(n−43)/2279⌋ + 1 ) – ( ⌊(n−53)/2279⌋ +
 *            1 ) + ( ⌊(n−96)/2279⌋ + 1 ) , with terms omitted when the
 *            numerator is negative. (All divisions are integer divisions on
 *            non‑negative values.)
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t P     = 43;
    const idx_t Q     = 53;
    const idx_t PQ    = 2279;
    const idx_t PQsum = P + Q;      /* 96 */

    seq_t res = (seq_t)(n / PQ) + 1;    /* 1 + floor(n/2279) */

    if (n >= P) {
        res -= (seq_t)((n - P) / PQ) + 1;
    }
    if (n >= Q) {
        res -= (seq_t)((n - Q) / PQ) + 1;
    }
    if (n >= PQsum) {
        res += (seq_t)((n - PQsum) / PQ) + 1;
    }

    return res;
}
