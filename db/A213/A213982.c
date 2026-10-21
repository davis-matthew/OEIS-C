/*
 * A213982  Least k >= 1 such that prime(n) +- k = 2^m * q, m >= 0, where q >=
 *          2 is prime.
 * https://oeis.org/A213982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= 1 such that prime(n) ± k = 2^m * q, where m >=
 *            0 and q >= 2 is prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"

static bool _is_form(idx_t y) {
    if (y < 2) return false;
    idx_t odd = y;
    /* Remove all factors of 2. */
    while ((odd & 1) == 0) odd >>= 1;
    /* If the remaining part is 1, y is a power of two, which qualifies
       because we can take q = 2 (prime) and appropriate m. */
    if (odd == 1) return true;
    /* Otherwise, accept only if the odd part is prime. */
    return is_prime(odd);
}

seq_t a(idx_t n) {
    /* Get the n-th prime (1‑based). */
    idx_t p = get_prime(n);

    for (idx_t k = 1; ; ++k) {
        /* Check prime - k if it stays >= 2. */
        if (k <= p - 2) {
            idx_t y = p - k;
            if (_is_form(y)) return (seq_t)k;
        }
        /* Check prime + k. */
        idx_t y = p + k;
        if (_is_form(y)) return (seq_t)k;
    }
}
