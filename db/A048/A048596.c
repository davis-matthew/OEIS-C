/*
 * A048596  Numbers k such that (k mod (sum of decimal digits of k)) divides
 *          k.
 * https://oeis.org/A048596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A048596: k such that k % sum_of_digits(k) != 0 and k % (k %
 *            sum_of_digits(k)) == 0 (i.e. the non‑zero remainder of k modulo
 *            its sum of decimal digits divides k)
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_sum.h"
#include "../utils/seq_realloc.h"

static seq_t *cache    = NULL;   /* cached sequence values           */
static size_t cap      = 0;      /* current capacity of cache        */
static size_t count    = 0;      /* number of values stored          */
static seq_t next_k    = 1;      /* next integer to test             */

/* Test whether k satisfies the property:
   r = k % sum_of_digits(k);  r != 0 and k % r == 0 */
static bool good(seq_t k)
{
    seq_t s = digit_sum(k);
    if (s == 0) return false;          /* should never happen for k>0   */
    seq_t r = k % s;
    if (r == 0) return false;          /* remainder 0 is not allowed    */
    return (k % r) == 0;              /* r divides k                     */
}

seq_t a(idx_t n)   /* n >= 1, offset 1 */
{
    /* Ensure at least n terms are computed */
    while (count < (size_t)n) {
        if (good(next_k)) {
            /* Grow cache if necessary */
            if (count == cap) {
                size_t new_cap = cap ? cap << 1 : 256;
                seq_t *tmp = seq_realloc(cache, cap, new_cap);
                cache = tmp;
                cap   = new_cap;
            }
            cache[count++] = next_k;   /* store found term */
        }
        ++next_k;                     /* advance to next candidate */
    }
    return cache[(size_t)n - 1];       /* return n-th term (1‑based)   */
}
