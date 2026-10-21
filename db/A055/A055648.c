/*
 * A055648  Integers in base 13 with each base 13 digit represented by 2
 *          decimal digits.
 * https://oeis.org/A055648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Σ_{i≥0} (n modulo 13 at step i) · 100^i i.e. reinterpret
 *            the base‑13 digits of n as digits in base 100. Example: n = 13 =
 *            (10)_13 → a(13) = 1·100 + 0 = 100
 * Range:     u8 n <= 38, u16 n <= 1182, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val  = 0;   /* accumulated result */
    seq_t base = 1;   /* 100^i */
    idx_t b = n;      /* working copy of n */

    while (b > 0) {
        idx_t digit = b % 13;           /* next base‑13 digit (least significant) */
        val += (seq_t)digit * base;     /* add contribution */
        base *= 100;                    /* next power of 100 */
        b    /= 13;                     /* shift to next digit */
    }

    return val;
}
