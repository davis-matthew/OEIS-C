/*
 * A088932  G.f.: 1/((1-x)^2*(1-x^2)*(1-x^4)*(1-x^8)).
 * https://oeis.org/A088932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8⌊n/4⌋⁴ + 8(m+8)⌊n/4⌋³ – 2(m³–6m²–19m–86)⌊n/4⌋² –
 *            8(m³–6m²–6m–22)⌊n/4⌋ – 7m³ + 42m² + 13m + 54 –
 *            (m³–6m²+5m+6)(–1)^⌊n/4⌋) / 48 where m = n mod 4
 * Range:     u8 n <= 17, u16 n <= 92, u32 n <= 1594, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /*  use 64‑bit signed intermediates – the values fit
        for n up to at least 10^5 (required by the test set). */
    int64_t q = (int64_t)(n / 4u);        /* floor(n/4)   */
    int64_t m = (int64_t)(n % 4u);        /* n mod 4      */

    int64_t m2 = m * m;                  /* m²           */
    int64_t m3 = m2 * m;                 /* m³           */

    int64_t term1 = 8 * q * q * q * q;
    int64_t term2 = 8 * (m + 8) * q * q * q;

    int64_t t3_coeff = m3 - 6 * m2 - 19 * m - 86;
    int64_t term3 = -2 * t3_coeff * q * q;

    int64_t t4_coeff = m3 - 6 * m2 - 6 * m - 22;
    int64_t term4 = -8 * t4_coeff * q;

    int64_t poly = m3 - 6 * m2 + 5 * m + 6;
    int64_t sign = (q % 2 == 0) ? 1 : -1;
    int64_t term5 = -poly * sign;

    int64_t poly_const = -7 * m3 + 42 * m2 + 13 * m + 54;

    int64_t sum = term1 + term2 + term3 + term4 + poly_const + term5;
    int64_t result = sum / 48;           /* exact division */

    return (seq_t)result;
}
