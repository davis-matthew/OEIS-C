/*
 * A070800  Smallest prime greater than phi(n): a(n) = nextprime(phi(n)).
 * https://oeis.org/A070800
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the smallest prime > φ(n), OEIS A070800 (offset 2)
 * Range:     u8 n <= 255, u16 n <= 8877, u32 n <= 8877, u64 n <= 8877,
 *            u128 n <= 8877, big any n
 * Helpers:   is_prime, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* behaviour for n < 2 is irrelevant */
    if (n < 2) return 0;

    /* compute φ(n) */
    seq_t phi_val = phi(n);

    /* search for the first prime > φ(n) */
    idx_t cand = (idx_t)phi_val + 1;
    while (!is_prime(cand))
        cand++;

    return (seq_t)cand;
}
