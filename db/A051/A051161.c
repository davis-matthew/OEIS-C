/*
 * A051161  a(n) is the exponent of n-th prime in the order (A003131) of the
 *          Monster simple group.
 * https://oeis.org/A051161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = exponent of the n‑th prime in the Monster group order; a(n)=0 for n>20 */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n > 20)
        return (seq_t)0;

    /* cache the primes up to the n‑th one */
    static idx_t primes[20000];
    static size_t count = 0;

    if (count == 0)
        primes[count++] = 2;

    while (count < n) {
        idx_t candidate = primes[count - 1] + 1;
        for (;; ++candidate) {
            int prime = 1;
            for (size_t i = 0; i < count; ++i) {
                idx_t p = primes[i];
                if ((p * p) > candidate)
                    break;
                if (candidate % p == 0) {
                    prime = 0;
                    break;
                }
            }
            if (prime) {
                primes[count++] = candidate;
                break;
            }
        }
    }

    idx_t p = primes[n - 1];
    switch (p) {
        case 2:   return (seq_t)46;
        case 3:   return (seq_t)20;
        case 5:   return (seq_t)9;
        case 7:   return (seq_t)6;
        case 11:  return (seq_t)2;
        case 13:  return (seq_t)3;
        case 17:
        case 19:
        case 23:
        case 29:
        case 31:
        case 41:
        case 47:
        case 59:
        case 71:  return (seq_t)1;
        default:  return (seq_t)0;
    }
}
