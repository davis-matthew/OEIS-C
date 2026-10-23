/*
 * A136696  Final nonzero digit of n! in base 9.
 * https://oeis.org/A136696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = last non‑zero digit of n! in base 9. For each i from 1 to
 *            n the factor 3 is removed from i (all powers), the remaining
 *            part is multiplied modulo 9. The total exponent of 3 in n! is
 *            v₃(n!) = ∑ floor(n/3ᵏ). Removing 3² = 9^t with t = ⌊v₃/2⌋ leaves
 *            an extra factor of 3 iff v₃(n!) is odd; that...
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t prod = 1;         /* product modulo 9 */

    /* multiply all numbers i with every factor of 3 removed   */
    for (idx_t i = 2; i <= n; ++i) {
        idx_t x = i;
        while (x % 3 == 0)   /* strip all 3‑powers from i   */
            x /= 3;
        prod = (prod * (x % 9)) % 9;
    }

    /* parity of the exponent of 3 in n! :  v₃(n!) mod 2 */
    bool odd = false;
    for (idx_t p = 3; p <= n; ) {
        odd ^= ((n / p) & 1);   /* flip if floor(n/p) is odd   */
        if (p > n / 3)          /* prevent overflow of p*=3    */
            break;
        p *= 3;
    }

    if (odd)                     /* re‑introduce a single factor of 3 */
        prod = (prod * 3) % 9;

    return prod;
}
