/*
 * A014740  Inverse of 731st cyclotomic polynomial.
 * https://oeis.org/A014740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = A(n) – A(n‑17) – A(n‑43) + A(n‑60) where A(k) = 0 for k <
 *            0 A(k) = floor(k/731) + 1 for k ≥ 0 This follows from the
 *            product representation 1/Φ_731(x) =
 *            (1‑x)⁻¹·(1‑x⁷³¹)⁻¹·(1‑x¹⁷)·(1‑x⁴³)
 * Range:     i8 n <= 773, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = ((seq_t)n) / 731 + 1;                /* A(n)           */

    if (n >= 17) {
        res -= (((seq_t)(n - 17)) / 731 + 1);        /* –A(n‑17)       */
    }
    if (n >= 43) {
        res -= (((seq_t)(n - 43)) / 731 + 1);        /* –A(n‑43)       */
    }
    if (n >= 60) {
        res += (((seq_t)(n - 60)) / 731 + 1);        /* +A(n‑60)       */
    }

    return res;
}
