/*
 * A039640  Fixed point of "k -> k/2 or (k+1)/2 until result is prime",
 *          starting with prime(n)-1.
 * https://oeis.org/A039640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the fixed point of the map k -> ceil(k/2) applied *
 *            repeatedly to k = prime(n)-1 until a prime (or 1) is reached. *
 *            For prime(n)-1 = 1 this gives a(n)=1. * * The sequence is
 *            defined for n >= 1. The routine tolerates * non‑increasing calls
 *            by always recomputing from the nth prime. * No memo...
 * Range:     u8 n <= 54, u16 n <= 6926, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* get_prime(idx_t n) -> uint32_t */
#include "../utils/is_prime.h"    /* is_prime(idx_t x) -> bool */

seq_t a(idx_t n)
{
    /* start from the (n‑th) prime minus one */
    seq_t k = (seq_t)get_prime(n) - 1;

    /* Special case: if the starting value is 1, it is already a fixed point. */
    if (k == (seq_t)1)
        return k;

    /* Apply the transform until we reach a prime or the value 1. */
    while (k > (seq_t)1 && !is_prime((idx_t)k)) {
        if ((k & (seq_t)1) == (seq_t)0)          /* k even */
            k = k / (seq_t)2;
        else                                     /* k odd */
            k = (k + (seq_t)1) / (seq_t)2;
    }

    return k;   /* either a prime or 1 */
}
