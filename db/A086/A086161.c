/*
 * A086161  Number of monomial ideals in two variables x, y that are Artinian,
 *          integrally closed, of colength n and contain x^2.
 * https://oeis.org/A086161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/3) + 1 + (n mod 3 == 2) (OEIS A086161)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute m = floor(n/3) using 64‑bit arithmetic. */
    idx_t m = n / 3;
    /* Start with m + 1. */
    seq_t res = (seq_t)m + 1;
    /* Add 1 when n mod 3 == 2. */
    if ((n % 3) == 2)
        res += 1;
    return res;
}
