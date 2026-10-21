/*
 * A089787  a(4n-3), a(4n-2), a(4n-1), and a(4n) are the units digit of the
 *          n-th prime followed by 1, 3, 7, and 9 respectively.
 * https://oeis.org/A089787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = units_digit(p_{ceil(n/4))) * 10 + [1,3,7,9][(n-1)%4]
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p_k denotes the k-th prime. */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* Map of suffixes for positions 1,2,3,4 within each block of four terms. */
    static const unsigned char suffix[4] = {1, 3, 7, 9};

    /* Index of the prime needed: ceil(n/4) = (n+3)/4 */
    idx_t prime_idx = (n + 3) / 4;

    /* Fetch the prime. The function returns a 32‑bit unsigned integer. */
    uint32_t prime = get_prime(prime_idx);

    /* Unit digit of the prime. */
    seq_t unit = (seq_t)(prime % 10);

    /* Position within the group: (n-1)%4 */
    unsigned pos = (unsigned)((n - 1) & 3);

    /* Construct the term: unit*10 + suffix. */
    seq_t result = unit * 10 + (seq_t)suffix[pos];
    return result;
}
