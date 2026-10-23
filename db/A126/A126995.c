/*
 * A126995  a(n) = binomial(prime(n+2), 3).
 * https://oeis.org/A126995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(prime(n+2), 3)
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 423, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* Index of the prime to use – OEIS uses 0‑based indexing for a(n) */
    idx_t p_index = n + (idx_t)2;

    /* Retrieve the (n+2)-th prime. get_prime is defined in get_prime.h. */
    uint64_t p = (uint64_t)get_prime(p_index);

    /* Compute C(p,3) = p*(p-1)*(p-2)/6.
       Perform the division early to stay within range of seq_t. */
    seq_t res = (seq_t)p;
    res = res * (p - 1u) / 2u;   /* (p*(p-1))/2 */
    res = res * (p - 2u) / 3u;   /* ((p*(p-1))/2*(p-2))/3 = C(p,3) */

    return res;
}
