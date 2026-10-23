/*
 * A184634  a(n) = floor(1/{(10+n^4)^(1/4)}), where {}=fractional part.
 * https://oeis.org/A184634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(1/{(10+n^4)^(1/4)}). The sequence is a cubic
 *            polynomial in k = floor(n/5), with different coefficients for
 *            each residue r = n mod 5. Derived formulas: a(5k) = 50k^3
 *            a(5k+1) = 50k^3 + 30k^2 + 6k a(5k+2) = 50k^3 + 60k^2 + 24k + 3
 *            a(5k+3) = 50k^3 + 90k^2 + 54k + 10 a(5k+4) = 50k^3 +120k...
 * Range:     u8 n <= 8, u16 n <= 54, u32 n <= 2206, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 3;
    if (n == 3) return 11;
    if (n == 4) return 25;

    idx_t r = n % 5;          /* residue 0..4 */
    seq_t k = 0;
    if (r == 0)
        k = (seq_t)(n / 5);
    else
        k = (seq_t)((n - r) / 5);

    seq_t term = 0;
    switch (r) {
        case 0: term = 50 * k * k * k; break;
        case 1: term = 50 * k * k * k + 30 * k * k + 6 * k; break;
        case 2: term = 50 * k * k * k + 60 * k * k + 24 * k + 3; break;
        case 3: term = 50 * k * k * k + 90 * k * k + 54 * k + 10; break;
        case 4: term = 50 * k * k * k + 120 * k * k + 96 * k + 25; break;
    }
    return term;
}
