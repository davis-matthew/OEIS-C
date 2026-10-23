/*
 * A121218  a(n) = (n-th prime)th positive integer which is coprime to n.
 * https://oeis.org/A121218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime-th positive integer that is coprime to n
 *            (search)
 * Range:     u8 n <= 23, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* nth prime (uint32_t) → seq_t */
    uint32_t p32 = get_prime(n);
    seq_t prime = (seq_t)p32;

    seq_t count = (seq_t)0;
    seq_t candidate = (seq_t)1;
    seq_t n_seq = (seq_t)n;

    while (1) {
        if (gcd(candidate, n_seq) == 1) {
            ++count;
            if (count == prime)
                break;
        }
        ++candidate;
    }

    return candidate;
}
