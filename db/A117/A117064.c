/*
 * A117064  Hexagonal numbers for which both the sum of the digits and the
 *          product of the digits are also hexagonal numbers.
 * https://oeis.org/A117064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 4442, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product, digit_sum, floor_sqrt, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A117064:
   a(n) = the n-th hexagonal number H_k = k(2k–1) such that
   digit_sum(H_k) and digit_product(H_k) are also hexagonal numbers. */

#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/floor_sqrt.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;          /* array of found terms */
static size_t cache_cap = 0;         /* current capacity */
static idx_t   count   = 0;          /* number of terms stored */
static idx_t   k_next  = 0;          /* next k to test */

/* Check if a value is hexagonal.  Hexagonal numbers are of the form
   m(2m-1).  Special case: 0 is hexagonal (m = 0). */
static inline bool is_hexagonal(seq_t x)
{
    if (x == 0) return true;
    seq_t D = 1 + 8 * x;            /* discriminant */
    seq_t sqrtD = floor_sqrt(D);
    if (sqrtD * sqrtD != D) return false;
    seq_t num = 1 + sqrtD;
    return (num % 4) == 0;
}

/* Compute the n-th term (n >= 1).  Calls are independent of order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0, return 0 */

    /* Generate terms until we have enough */
    while (count < n) {
        seq_t k  = (seq_t)k_next;
        seq_t H  = k * ((seq_t)2 * k - 1);     /* k(2k–1) */

        seq_t sum  = digit_sum(H);
        seq_t prod = digit_product(H);

        if (is_hexagonal(sum) && is_hexagonal(prod)) {
            /* grow cache if needed */
            if (count >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[count++] = H;
        }
        ++k_next;
    }
    return cache[n-1];
}
