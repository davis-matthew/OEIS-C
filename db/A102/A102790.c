/*
 * A102790  Integer part of n#/(p-3)#, where p=preceding prime to n.
 * https://oeis.org/A102790
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) * (prime(n)-2 if prime(n)-2 is prime else 1)
 * Range:     u8 n <= 7, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    // undefined for n < 2, return 0 for safety
    if (n < 2) return (seq_t)0;

    // Obtain nth prime
    seq_t p = get_prime((uint64_t)n);

    // Convert to uint64_t to test for primality of p-2
    uint64_t p_val   = (uint64_t)p;
    uint64_t pm2_val = (p_val > 2) ? p_val - 2 : 0;

    // If p-2 is prime, multiply by it; otherwise return p
    if (pm2_val > 2 && is_prime(pm2_val))
        return p * (seq_t)pm2_val;
    else
        return p;
}
