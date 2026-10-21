/*
 * A240659  Least k such that 4^k == -1 (mod prime(n)), or 0 if no such k
 *          exists.
 * https://oeis.org/A240659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS formula: a(n) = A082654(n)/2 if A082654(n) is even, otherwise 0.
   Here A082654(n) is the multiplicative order of 4 modulo the nth prime.
   The function returns 0 when 4^k ≡ -1 (mod prime(n)) has no solution.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_prime.h"      /* get_prime(idx_t) -> uint32_t */
#include "../utils/mod_pow.h"        /* mod_pow(seq_t, uint64_t, seq_t) */

/* Compute a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0U)          /* below the offset */
        return 0;

    /* nth prime (p).  get_prime(k) returns a 32‑bit unsigned int. */
    uint32_t p = get_prime(n);
    if (p == 2U)          /* 4^k ≡ -1 (mod 2) impossible */
        return 0;

    /* Order of 4 modulo p divides p‑1.  Work with 64‑bit integers
       because p ≤ 104,729 for n ≤ 10 000; the order is ≤ p‑1 and
       comfortably fits in 64 bits. */
    uint64_t order = (uint64_t)p - 1ULL;
    uint64_t phi   = order;            /* φ(p) = p‑1 */

    /* Factor φ(p) by trial division (φ ≤ 104,728 → √φ ≤ 324). */
    uint64_t temp = phi;
    /* factor 2 */
    while ((temp & 1ULL) == 0ULL) {
        /* try to reduce order by 2 */
        while (order % 2ULL == 0ULL &&
               mod_pow((seq_t)4, order / 2ULL, (seq_t)p) == 1)
            order /= 2ULL;
        temp >>= 1ULL;      /* temp /= 2 */
    }
    /* odd factors starting at 3 */
    for (uint64_t f = 3ULL; f * f <= temp; f += 2ULL) {
        if (temp % f == 0ULL) {
            /* f is a prime factor of φ(p) */
            while (temp % f == 0ULL)
                temp /= f;
            /* reduce order using this prime factor */
            while (order % f == 0ULL &&
                   mod_pow((seq_t)4, order / f, (seq_t)p) == 1)
                order /= f;
        }
    }
    /* if a remaining prime factor > sqrt(phi) */
    if (temp > 1ULL) {
        uint64_t f = temp;
        while (order % f == 0ULL &&
               mod_pow((seq_t)4, order / f, (seq_t)p) == 1)
            order /= f;
    }

    /* If the order is even, the least k with 4^k ≡ -1 is order/2. */
    return (order & 1ULL) ? (seq_t)0 : (seq_t)(order / 2ULL);
}
