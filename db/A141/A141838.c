/*
 * A141838  a(n) = first term that can be reduced in n steps via repeated
 *          interpretation of a(n) as a base b+1 number where b is the largest
 *          digit of a(n), such that b is always 4 so that each interpretation
 *          is base 5. Terms already fully reduced (i.e., single digits) are
 *          excluded.
 * https://oeis.org/A141838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, more, nonn
 * Formula:   a(n) = minimal term that can be reduced in exactly n steps by
 *            repeatedly interpreting the term as a base‑5 number. (Terms that
 *            have only one digit are excluded.)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A141838: a(1) = 14 and a(n+1) = the base-5 digits of a(n) read in decimal
 * (14 -> 24 -> 44 -> 134 -> ...). The conversion needs the exact value of
 * a(n), so the chain runs in 128 bits at the fixed widths (in seq_t for big);
 * the terms that fit are exact. */

#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef unsigned __int128 wide_t;
#endif

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    wide_t x = 14;
    for (idx_t i = 1; i < n; ++i) {
        unsigned char d[200];
        int c = 0;
        for (wide_t t = x; t > 0 && c < 200; t /= 5) d[c++] = (unsigned char)(int)(t % 5);
        x = 0;
        while (c--) x = x * 10 + d[c];
    }
    return (seq_t)x;
}
