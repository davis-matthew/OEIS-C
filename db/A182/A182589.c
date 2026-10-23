/*
 * A182589  Number of p-gons in cubic curve, where p = n-th prime.
 * https://oeis.org/A182589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*(2^(p-2)+1)*(2^(p-1)-1)/p where p = n-th prime
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=8, a(2)=24, a(3)=216, ... */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                  /* offset 1 – below it is irrelevant */
    uint32_t prime = get_prime(n);         /* nth prime (1‑based) */
    seq_t p = (seq_t)prime;

    seq_t two_p_minus2 = pow2((idx_t)(prime - 2));
    seq_t part1 = two_p_minus2 + (seq_t)1;             /* 2^(p-2)+1 */
    seq_t two_p_minus1 = pow2((idx_t)(prime - 1));
    seq_t part2 = two_p_minus1 - (seq_t)1;             /* 2^(p-1)-1 */

    seq_t prod   = part1 * part2;                     /* (2^(p-2)+1)*(2^(p-1)-1) */
    seq_t result = ((seq_t)8 * prod) / p;             /* 8 * prod / p */

    return result;
}
