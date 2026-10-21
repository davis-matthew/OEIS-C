/*
 * A070802  a(n)=prevprime[sigma(n)]-nextprime[phi(n)]=A070801(n)-A070800(n).
 * https://oeis.org/A070802
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prevprime[sigma(n)] - nextprime[phi(n)]
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   is_prime_local, next_prime_of, phi, prev_prime_of, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/phi.h"
#include "../utils/is_prime_local.h"
#include "../utils/next_prime_of.h"
#include "../utils/prev_prime_of.h"

/* OEIS A070802 */
seq_t a(idx_t n)
{
    /* Works for n >= 2; caller guarantees offset */
    seq_t sigma_n = sigma_of(n);   /* sum of divisors */
    seq_t phi_n   = phi(n);        /* Euler's totient */

    uint64_t s = (uint64_t)sigma_n;
    uint64_t p = (uint64_t)phi_n;

    uint64_t prev = prev_prime_of(s);   /* prevprime[sigma(n)] */
    uint64_t next = next_prime_of(p);   /* nextprime[phi(n)] */

    return (seq_t)(prev - next);
}
