/*
 * A018742  Divisors of 956.
 * https://oeis.org/A018742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 956 in increasing order (offset 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1U) return 0;          /* undefined for n < 1 */
    /* 956 = 2^2 * 239^1 */
    const int exp2 = 2;            /* exponent of 2 */
    const int exp239 = 1;          /* exponent of 239 */
    const seq_t p239 = (seq_t)239;
    seq_t divs[6];
    idx_t cnt = 0;

    /* Generate all combinations of exponents */
    for (int a = 0; a <= exp2; ++a) {
        seq_t v2 = ((seq_t)1) << a;   /* 2^a */
        for (int b = 0; b <= exp239; ++b) {
            seq_t v239 = (b == 0 ? (seq_t)1 : p239);
            divs[cnt++] = v2 * v239;
        }
    }

    /* Simple insertion sort for the 6-element array */
    for (idx_t i = 0; i < cnt; ++i) {
        for (idx_t j = i + 1; j < cnt; ++j) {
            if (divs[j] < divs[i]) {
                seq_t temp = divs[i];
                divs[i] = divs[j];
                divs[j] = temp;
            }
        }
    }

    if (n > 6U) return 0;          /* no term beyond the largest divisor */
    return divs[n - 1];
}
