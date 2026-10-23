/*
 * A171036  In the sequence of prime numbers, replace all the '7' digits with
 *          '2' and vice versa.
 * https://oeis.org/A171036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = replace the decimal digits 2 and 7 in the n‑th prime.
 *            Implementation uses get_prime(idx_t) to obtain the n‑th prime
 *            and then performs the digit swap on the resulting integer
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, replace_27
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/replace_27.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for n = 0 */

    /* Retrieve the n-th prime (1‑based). */
    uint32_t prime32 = get_prime(n);

    /* Convert to seq_t (may truncate if seq_t is bigger). */
    seq_t prime = (seq_t)prime32;

    /* Apply the 2↔7 digit swap. */
    return replace_27(prime);
}
