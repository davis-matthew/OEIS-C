/*
 * A187866  Greatest k such that prime(n)*(prime(n)-k)-1 and
 *          prime(n)*(prime(n)-k)+1 are twin primes, k >= 0 and k < prime(n)
 *          or -1 if no such k exists.
 * https://oeis.org/A187866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 609, i32 n <= 609, i64 n <= 9990,
 *            i128 n <= 9990, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = greatest k (0 ≤ k < prime(n)) such that
 *                prime(n)*(prime(n)-k)-1  and  prime(n)*(prime(n)-k)+1
 *                are both prime; returns -1 if no such k exists.
 */

#include <stdbool.h>
#include <stdint.h>

#include "../utils/get_prime.h"      /* extern uint32_t get_prime(idx_t n); */
#include "../utils/is_prime_big.h"   /* extern bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)-1;          /* undefined for n < 1 */

    uint32_t p_uint = get_prime(n);   /* nth prime (1‑based), fits in uint32_t */
    uint64_t p = p_uint;              /* promote to 64‑bit for loop */

    for (uint64_t y = 1; y <= p; ++y) {
        seq_t y_seq = (seq_t)y;
        seq_t prod  = (seq_t)p * y_seq;   /* p*y */

        seq_t n1 = prod - 1;              /* p*y - 1 */
        seq_t n2 = prod + 1;              /* p*y + 1 */

        if (is_prime_big(n1) && is_prime_big(n2)) {
            return (seq_t)(p - y);        /* k = p - y */
        }
    }

    return (seq_t)-1;                      /* no such k */
}
