/*
 * A055124  Base-14 complement of n (write n in base 14, then replace each
 *          digit with its base-14 negative).
 * https://oeis.org/A055124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Σ ((14 - (n // 14^i mod 14)) mod 14) * 14^i (i ≥ 0). This
 *            is the base‑14 complement of n: write n in base‑14, and replace
 *            each digit d by (14‑d) mod 14
 * Range:     u8 n <= 195, u16 n <= 2743, u32 n <= 2743, u64 n <= 2743,
 *            u128 n <= 2743, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t mult   = 1;
    idx_t t      = n;

    while (t > 0) {
        idx_t d   = t % 14;               /* current digit in base 14 */
        int comp  = (14 - (int)d) % 14;    /* complement digit   */
        result += (seq_t)comp * mult;      /* add digit contribution */
        mult *= (seq_t)14;                 /* advance multiplier  */
        t /= 14;                           /* next digit         */
    }

    return result;
}
