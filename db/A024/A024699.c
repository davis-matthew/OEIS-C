/*
 * A024699  a(n) = (prime(n+2)-1)/6 if this is an integer or (prime(n+2)+ 1)/6
 *          otherwise.
 * https://oeis.org/A024699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((prime(n+2)+3)/6)
 * Range:     u8 n <= 52, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1: a(1) corresponds to the first term. */
    if (n < 1) return 0;  /* undefined for n == 0, but return 0 for safety */

    /* get_prime(k) returns the k‑th prime (1‑based). */
    uint64_t p = get_prime(n + 2);   /* prime(n+2) */
    return ((seq_t)p + 3) / 6;       /* floor((p+3)/6) */
}
