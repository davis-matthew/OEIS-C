/*
 * A193275  Expansion of psi(x^2) * phi(x^17) - x^4 * phi(x) * psi(x^34) in
 *          powers of x^4 where phi(), psi() are Ramanujan theta functions.
 * https://oeis.org/A193275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^{4n} in ψ(x²) * φ(x¹⁷) – x⁴ * φ(x) *
 *            ψ(x³⁴), n ≥ 0
 * Range:     i8 n <= 381, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   ψ(q) = ∑_{k≥0} q^{k(k+1)/2}, φ(q) = ∑_{k∈ℤ} q^{k²}        */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* First product: ψ(x²) * φ(x¹⁷)
       Exponent = a(a+1) + 17·b²,  coefficient = 1 if b = 0 else 2   */
    for (idx_t a = 0; ; ++a) {
        seq_t exp_a = (seq_t)a * (a + 1);
        if (exp_a > n) break;          /* further a give larger exponents */
        for (idx_t b = 0; ; ++b) {
            seq_t exp_b = (seq_t)17 * b * b;
            seq_t e = exp_a + exp_b;
            if (e > n) break;
            if (e == n) {
                seq_t coeff = (b == 0) ? 1 : 2;
                result += coeff;
            }
        }
    }

    /* Second product: x⁴ * φ(x) * ψ(x³⁴)
       Exponent = 4 + c² + 17·d(d+1),  coefficient = 1 if c = 0 else 2   */
    if (n >= 4) {
        for (idx_t d = 0; ; ++d) {
            seq_t exp_d = (seq_t)17 * d * (d + 1);
            if ((seq_t)4 + exp_d > n) break;
            for (idx_t c = 0; ; ++c) {
                seq_t exp_c = (seq_t)c * c;
                seq_t e = (seq_t)4 + exp_d + exp_c;
                if (e > n) break;
                if (e == n) {
                    seq_t coeff = (c == 0) ? 1 : 2;
                    result -= coeff;
                }
            }
        }
    }

    return result;
}
