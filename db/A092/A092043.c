/*
 * A092043  a(n) = numerator(n!/n^2).
 * https://oeis.org/A092043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = (n-1)! / gcd(n,(n-1)!) (equals (n-1)! if n is prime,
 *            (n-1)!/n for composite n>4, and 3 for n=4)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"

/*
 * Compute the numerator of n!/n^2, which is the integer
 * (n-1)! divided by gcd(n,(n-1)!).
 * The function works for all supported seq_t sizes:
 *   • If seq_t has 128‑bit or larger (i.e. fits arbitrary precision),
 *     the calculation is performed using seq_t arithmetic, guaranteeing
 *     full precision.
 *   • For 64‑bit or smaller seq_t, an intermediate __int128 value
 *     is used so that intermediate products up to 22! fit; this covers
 *     all n whose results fit in 64‑bits.
 */

seq_t a(idx_t n)
{
    /* offset 1: a(1) = 1 */
    if (n <= 1)
        return (seq_t)1;

    /* Special case n == 4 gives 3, not (3!)/4 */
    if (n == 4)
        return (seq_t)3;

    /* For 128‑bit or larger seq_t types use seq_t arithmetic */
    if (sizeof(seq_t) >= 16) {
        seq_t prod = 1;
        for (idx_t k = 2; k < n; ++k)
            prod *= k;
        return is_prime(n) ? prod : prod / n;
    }

    /* For 64‑bit or smaller seq_t use an intermediate __int128 */
    __int128 prod = 1;
    for (idx_t k = 2; k < n; ++k)
        prod *= k;

    if (is_prime(n))
        return (seq_t)prod;
    else
        return (seq_t)(prod / n);
}
