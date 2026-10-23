/*
 * A021478  Decimal expansion of 1/474.
 * https://oeis.org/A021478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^n / 474) mod 10
 * Range:     u8 n <= 4, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t remainder = 1;   /* current remainder of 1/474 after n digits */
    seq_t digit     = 0;   /* digit at position n */

    idx_t i;
    for (i = 0; i <= n; ++i) {
        /* use 64‑bit arithmetic to avoid overflow in seq_t that may
           be as small as uint8_t */
        uint64_t tmp = (uint64_t)remainder * 10ULL;

        digit     = (seq_t)(tmp / 474ULL);
        remainder = (seq_t)(tmp % 474ULL);
    }

    return digit;
}
