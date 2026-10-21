/*
 * A075771  Let n^2 = q*prime(n) + r with 0 <= r < prime(n); then a(n) = q +
 *          r.
 * https://oeis.org/A075771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 - (prime(n)-1)*floor(n^2/prime(n))
 * Range:     u8 n <= 15, u16 n <= 353, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently a(n) = q + r where n^2 = q*prime(n) + r) */

#include <stdint.h>             /* for uint32_t */
#include "../utils/get_prime.h" /* provides uint32_t get_prime(idx_t k); */

seq_t a(idx_t n) {
    /* n must be >= 1 (offset 1) */
    seq_t n_sq = (seq_t)n * (seq_t)n;          /* compute n^2 in seq_t */
    uint32_t p_uint = get_prime(n);            /* n‑th prime, fits in uint32_t */
    seq_t prime = (seq_t)p_uint;              /* promote to seq_t */
    seq_t q = n_sq / prime;                   /* integer division */
    seq_t r = n_sq % prime;                   /* remainder */
    return q + r;                             /* a(n) = q + r */
}
