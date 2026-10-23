/*
 * A171610  a(n) = 0+1+2+...+n in lunar arithmetic in base 7 written in base
 *          7.
 * https://oeis.org/A171610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{k=0}^{⌊log₇(n)⌋} min(6, ⌊n/7ᵏ⌋) * 10ᵏ which equals the
 *            lunar sum 0+1+…+n in base 7, written with the base‑7 digits
 *            interpreted as decimal digits
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;           /* result as decimal integer   */
    seq_t power10 = 1;       /* current power of 10 (decimal) */
    idx_t power7 = 1;        /* current power of 7 (base 7)  */

    while (power7 <= n) {
        idx_t d = n / power7;        /* digit in base‑7 place k   */
        if (d > 6) d = 6;            /* digits are 0–6 only     */
        res += (seq_t)d * power10;   /* accumulate decimal digit */

        /* Prepare for next higher base‑7 digit */
        if (power7 > n / 7) break;   /* next power would exceed n */
        power7 *= 7;
        power10 *= 10;
    }
    return res;
}
