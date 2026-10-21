/*
 * A090939  Least multiple of n == -1 (mod prime(n)).
 * https://oeis.org/A090939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * ((prime(n)-1) * inv(n mod prime(n)) mod prime(n)),
 *            where inv is the modular inverse of n modulo the n-th prime
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   egcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/egcd.h"

seq_t a(idx_t n)
{
    uint64_t p = get_prime(n);            /* the n-th prime, p > n for all n >= 1 */
    long long n_ll = (long long)n;
    long long p_ll = (long long)p;

    /* modular inverse of n modulo p via extended Euclid */
    long long inv, dummy;
    egcd(n_ll, p_ll, &inv, &dummy);
    if (inv < 0) inv += p_ll;              /* make it positive */

    /* k = ((p-1) * inv) % p, but k = p if the residue is 0   */
    uint64_t k = ((uint64_t)(p - 1) * (uint64_t)inv) % p;
    if (k == 0) k = p;

    return (seq_t)n * (seq_t)k;             /* result fits in seq_t for n <= 10000 */
}
