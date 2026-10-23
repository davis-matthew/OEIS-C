/*
 * A024698  a(n) = (prime(n+1) - 1)/4 if this is an integer or (prime(n+1) +
 *          1)/4 otherwise.
 * https://oeis.org/A024698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil((prime(n+1)-1)/4) = (prime(n+1)-1)/4 if
 *            prime(n+1)%4==1, otherwise (prime(n+1)+1)/4. The implementation
 *            uses the provided get_prime utility which returns the k‑th prime
 *            (k>=1). No global state or dynamic allocation is required
 * Range:     u8 n <= 53, u16 n <= 7148, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t
a(idx_t n)
{
    /* OFFSET is 1; for n==0 return 0 as a defensive default. */
    if (n == 0)
        return (seq_t)0;

    /* Retrieve prime(n+1) */
    uint32_t p32 = get_prime(n + 1);
    seq_t p = (seq_t)p32;

    if (p % (seq_t)4 == (seq_t)1)
        return (p - (seq_t)1) / (seq_t)4;
    else
        return (p + (seq_t)1) / (seq_t)4;
}
