/*
 * A059018  Write 10*n in base 4; a(n) = sum of digits mod 4.
 * https://oeis.org/A059018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum of the base‑4 digits of 10 × n, taken modulo 4
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 10 × n can need up to 68 bits, so use 128‑bit arithmetic. */
    unsigned __int128 v = (unsigned __int128)n * 10u;
    seq_t s = 0;

    /* each iteration extracts a base‑4 digit by masking the lowest two bits */
    while (v > 0) {
        s += (seq_t)(v & 3u);  /* v % 4 */
        v >>= 2;               /* v /= 4 */
    }
    return s % 4;
}
