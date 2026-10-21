/*
 * A096060  (2^(p-1)-1)/(3*p) where p = prime(n).
 * https://oeis.org/A096060
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(p-1)-1) / (3*p), where p = prime(n)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 12, u64 n <= 20, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 3.  For smaller indices return 0. */
    if (n < 3)
        return 0;

    /* Get the n‑th prime.  get_prime returns a 32‑bit integer. */
    uint64_t p = get_prime(n);

    /* For 64‑bit or smaller seq_t we can compute 2^(p-1) exactly in a
       128‑bit temporary when p-1 <= 127.  This covers all cases where
       the final result fits in seq_t. */
    if (sizeof(seq_t) <= sizeof(unsigned long long) && p <= 127)
    {
        /* power = 2^(p-1)  (fits in 128 bits) */
        unsigned __int128 power = ((unsigned __int128)1) << (p - 1);
        unsigned __int128 numerator = power - 1;
        unsigned __int128 denominator = ((unsigned __int128)3) * p;
        return (seq_t)(numerator / denominator);
    }
    else
    {
        /* Use the arbitrary‑precision helper for larger p or larger
           seq_t types. */
        seq_t power = big_pow((seq_t)2, (seq_t)(p - 1));
        seq_t numerator = power - 1;
        seq_t denominator = (seq_t)3 * p;
        return numerator / denominator;
    }
}
