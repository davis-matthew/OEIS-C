/*
 * A134451  Ternary digital root of n.
 * https://oeis.org/A134451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, mult
 * Formula:   a(n) = n if n <= 2, otherwise a(A053735(n))
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9951 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A053735(n) is the sum of the base‑3 digits of n. */
seq_t a(idx_t n)
{
    /* a(0) is defined as 0, and for 1 <= n <= 2 the value is n itself. */
    if (n == 0)
        return (seq_t)0;

    /* Reduce n by repeatedly replacing it with the sum of its base‑3 digits
       until the result is 0, 1 or 2. */
    while (n > 2)
    {
        idx_t sum = 0;
        idx_t t = n;
        while (t != 0)
        {
            sum += t % 3;
            t   /= 3;
        }
        n = sum;
    }

    return (seq_t)n;
}
