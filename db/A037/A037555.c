/*
 * A037555  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,1.
 * https://oeis.org/A037555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037555: Base‑6 digits 2,1,1 repeating.
   a(1) = 2
   For n ≥ 2: a(n) = a(n‑1) * 6 + d(n)
   where d(n) = 2 if (n‑1) mod 3 == 0, otherwise 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n<1, but harmless */
    seq_t result = 2;           /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t digit = ((i - 1) % 3 == 0) ? 2 : 1;
        result = result * 6 + digit;
    }
    return result;
}
