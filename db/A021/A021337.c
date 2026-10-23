/*
 * A021337  Decimal expansion of 1/333.
 * https://oeis.org/A021337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = 1 + (n mod 3) - ((n+1) mod 3)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplified: a(n) = 3 if n mod 3 = 2, else 0. */
seq_t a(idx_t n)
{
    /* Compute n mod 3 using the unsigned index type. */
    idx_t r = n % 3;
    if (r == 2)
        return 3;   /* seq_t automatically promotes the literal */
    else
        return 0;
}
