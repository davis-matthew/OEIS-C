/*
 * A024863  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (natural numbers), t = A000201 (lower Wythoff sequence).
 * https://oeis.org/A024863
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊n/2⌋} i * ⌊(n‑i+1) · (1+√5)/2⌋
 * Range:     u8 none, u16 none, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer approximation of √5 with denominator 1 000 000,
   enough to compute the floor exactly for n ≤ 10 000. */
static const idx_t _LOWER_WYTHOFF_NUM = 3236068;   /* 2·10⁶ · √5 + 2·10⁶ ≈ 3 236 068 */
static const idx_t _LOWER_WYTHOFF_DEN = 2000000;   /* 2·10⁶ */

seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* below offset – undefined for the sequence */

    idx_t k = n / 2;                    /* floor(n/2) */
    seq_t sum = 0;

    for (idx_t i = 1; i <= k; ++i) {
        idx_t j = n - i + 1;
        /* floor(j·φ) = floor(j·(1+√5)/2) ≈ floor(j·3236068/2000000) */
        seq_t t = (seq_t)(j * _LOWER_WYTHOFF_NUM) / (seq_t)_LOWER_WYTHOFF_DEN;
        sum += (seq_t)i * t;
    }
    return sum;
}
