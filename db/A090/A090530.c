/*
 * A090530  Least multiple k of prime(n) such that (k-1,k+1) forms a twin
 *          prime pair, or 0 if no such number exists.
 * https://oeis.org/A090530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least multiple k of prime(n) such that (k-1,k+1) form a
 *            twin prime pair. If no such k exists, a(n) = 0
 * Range:     u8 n <= 5, u16 n <= 65, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; undefined for n=0 */
    
    uint32_t p_u32 = get_prime(n);        /* p = nth prime */
    seq_t p = (seq_t)p_u32;
    
    seq_t m = 1;
    while (1) {
        seq_t k        = p * m;           /* candidate multiple */
        seq_t k_minus  = k - 1;
        seq_t k_plus   = k + 1;
        
        if (isPrimeGeneric(k_minus) && isPrimeGeneric(k_plus))
            return k;                     /* twin primes found */
        
        ++m;                              /* try next multiple */
    }
}
