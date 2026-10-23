/*
 * A038953  Primes p such that 70 is a square mod p.
 * https://oeis.org/A038953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, jacobi_symbol, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS sequence a(n) lists primes p such that 70 is a quadratic residue mod p.
   This implementation generates successive primes with next_prime() and keeps
   those for which the Legendre/Jacobi symbol (70|p) equals 1 or 0 (the latter
   occurs when p divides 70).  Prime 2 is handled explicitly. */
#include "../utils/jacobi_symbol.h"
#include "../utils/next_prime.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset handling */
    if (n == 1) return (seq_t)2;         /* first term is 2 */
    idx_t count = 1;                     /* we already have prime 2 */
    idx_t prime = 3;                     /* start searching from 3 */
    while (count < n) {
        if (is_prime(prime)) {
            int js = jacobi_symbol(70ULL, prime);
            if (js == 1 || js == 0) {    /* up to 70 is a quadratic residue */
                count++;
                if (count == n) return (seq_t)prime;
            }
        }
        prime = next_prime(prime + 1);
    }
    return (seq_t)0;                     /* should never be reached */
}
