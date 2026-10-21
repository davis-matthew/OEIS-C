/*
 * A085980  Number of 6's in decimal expansion of prime(n).
 * https://oeis.org/A085980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   number of digit '6' in decimal expansion of prime(n)
 * Range:     u8 n <= 55, u16 n <= 7546, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Obtain the n‑th prime (1‑based index).  The helper returns a
       32‑bit value; cast it to seq_t for further arithmetic. */
    seq_t prime = (seq_t)get_prime(n);

    /* Count the occurrences of the digit 6. */
    seq_t count = 0;
    seq_t ten   = (seq_t)10;

    while (prime != 0)
    {
        if (prime % ten == 6)
            count++;
        prime /= ten;
    }

    return count;
}
