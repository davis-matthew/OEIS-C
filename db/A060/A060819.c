/*
 * A060819  a(n) = n / gcd(n,4).
 * https://oeis.org/A060819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n / gcd(n,4)
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) = n / gcd(n,4) for n >= 1.
 * Uses simple case‑based arithmetic that avoids any external helper.
 */
seq_t a(idx_t n)
{
    /* Handle n == 0 just in case; not part of the defined sequence. */
    if (n == 0) return 0;

    /* Fast computation based on n % 4 */
    idx_t mod4 = n & 3ULL;          /* equivalent to n % 4 for unsigned 64‑bit */
    if (mod4 == 0)                  /* n divisible by 4 */
        return (seq_t)(n / 4);
    if ((mod4 & 1ULL) == 0)         /* n even but not divisible by 4 */
        return (seq_t)(n / 2);
    return (seq_t)n;                /* n odd */
}
