/*
 * A048767  If n = Product (p_j^k_j) then a(n) = Product ( prime(k_j)^pi(p_j)
 *          ) where pi is A000720.
 * https://oeis.org/A048767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} prime(e)^π(p) (π is the prime‑counting
 *            function)
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 130, u64 n <= 310,
 *            u128 n <= 718, big any n
 * Helpers:   big_pow, compute_pi, factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"
#include "../utils/compute_pi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    /* Maximum number of distinct prime factors of a 64‑bit integer is 16. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t pcnt = 0;

    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < pcnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        seq_t base = (seq_t)get_prime((idx_t)e);        /* prime(e)  */
        seq_t exp  = compute_pi((idx_t)p);              /* π(p)      */

        result = result * big_pow(base, exp);
    }
    return result;
}
