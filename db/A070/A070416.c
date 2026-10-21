/*
 * A070416  a(n) = 7^n mod 32.
 * https://oeis.org/A070416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n mod 32. Since 7^4 ≡ 1 (mod 32), the sequence repeats
 *            every 4 terms: 1, 7, 17, 23, 1, …
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use only 2‑sized integer, safe for any seq_t implementation */
    int r = (int)(n & 3);          /* n mod 4, fits in a plain int */

    switch (r)
    {
        case 0: return (seq_t)1;   /* 7^0 mod 32 */
        case 1: return (seq_t)7;   /* 7^1 mod 32 */
        case 2: return (seq_t)17;  /* 7^2 mod 32 */
        case 3: return (seq_t)23;  /* 7^3 mod 32 */
    }
    /* unreachable, keeps compiler happy */
    return (seq_t)0;
}
