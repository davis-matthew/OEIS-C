/*
 * A021018  Decimal expansion of 1/14.
 * https://oeis.org/A021018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal expansion of 1/14.
   a(0) = 0; for n≥1, a(n) = digit at (n)th position after the decimal point,
   i.e. the repeating cycle 7,1,4,2,8,5. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* compute (n-1) mod 6 to get position in the cycle */
    idx_t idx = (n - 1) % 6;
    switch (idx) {
        case 0: return (seq_t)7;
        case 1: return (seq_t)1;
        case 2: return (seq_t)4;
        case 3: return (seq_t)2;
        case 4: return (seq_t)8;
        case 5: return (seq_t)5;
    }
    return (seq_t)0;  /* unreachable, satisfies compiler */
}
