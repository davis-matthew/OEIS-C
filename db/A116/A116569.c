/*
 * A116569  a(n) = (x^3 - x) / 6, where x is the genus of the modular curve
 *          X_0(p) for p = prime(n).
 * https://oeis.org/A116569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (x^3 - x) / 6, where x is the genus of the modular curve
 *            X0(p_n) * for the n-th prime p_n. The genus for a prime p > 3 is
 *            * * g = floor((p + 1) / 12) , * with the adjustment g -= 1 if p
 *            > 13 and p ≡ 1 (mod 12)
 * Range:     u8 n <= 34, u16 n <= 153, u32 n <= 2228, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Retrieve the n-th prime (1‑based index). */
    uint64_t p = get_prime(n);

    /* Compute the genus of X0(p). */
    uint64_t g_val = (p + 1) / 12;
    if (p > 13u && (p % 12u) == 1u)
        g_val -= 1u;
    seq_t g = (seq_t)g_val;

    /* Compute a(n) = (g^3 - g) / 6. */
    seq_t res = (g * g * g - g) / 6;
    return res;
}
