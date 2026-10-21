/*
 * A091072  Positive numbers k such that the Kronecker Symbol (-1 / k) > 0.
 * https://oeis.org/A091072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose odd part ≡ 1 (mod 4)  (OEIS A091072) */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* If n == 0, return 0 as a safe undefined value. */
    if (n == 0) return (seq_t)0;

    idx_t found = 0;   /* number of terms found so far   */
    idx_t k     = 0;   /* current candidate integer     */

    while (found < n) {
        ++k;                 /* move to next integer         */
        idx_t odd = k;       /* strip factors of 2           */
        while ((odd & 1ULL) == 0)
            odd >>= 1;

        /* odd part ≡ 1 (mod 4) ⇔ lowest two bits of odd are 01 */
        if ((odd & 3ULL) == 1)
            ++found;
    }
    return (seq_t)k;
}
