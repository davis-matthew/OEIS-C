/*
 * A062874  Integer part of arithmetic mean of n! and n^n.
 * https://oeis.org/A062874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ⌊(n! + nⁿ)/2⌋
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* factorial n! as 128‑bit integer */
    unsigned __int128 fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (unsigned __int128)i;

    /* nⁿ as 128‑bit integer (binary exponentiation) */
    unsigned __int128 powv = 1;
    unsigned __int128 base = n;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1U)
            powv *= base;
        base *= base;
        exp >>= 1U;
    }

    /* floor((fact + powv)/2) */
    return (seq_t)((fact + powv) >> 1);
}
