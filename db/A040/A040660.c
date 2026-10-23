/*
 * A040660  Continued fraction for sqrt(687).
 * https://oeis.org/A040660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040660: continued fraction coefficients for sqrt(687) */
/* a(0) = 26; for n >= 1, a(n) = period[(n-1) mod 10] with */
/* period = {4, 1, 2, 1, 16, 1, 2, 1, 4, 52} */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* The period of the continued fraction for sqrt(687) */
    static const seq_t period[10] = {4, 1, 2, 1, 16, 1, 2, 1, 4, 52};

    if (n == 0)
        return (seq_t)26;

    /* (n - 1) is safe because the branch for n == 0 already handled */
    idx_t k = (n - 1) % (idx_t)10;           /* 10 is the period length   */
    return period[(size_t)k];
}
