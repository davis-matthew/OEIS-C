/*
 * A089933  Concatenate the first n odd primes.
 * https://oeis.org/A089933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the first n odd primes (n >= 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   digits_uint64, get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/digits_uint64.h"
#include "../utils/pow10.h"

/* Cache for sequence values */
static seq_t *seq_cache = NULL;
static size_t  seq_cap  = 0;
static size_t  seq_len  = 0;

/* Ensure capacity for seq_cache */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed)
        return;
    if (seq_cap == 0)
        seq_cap = 16;
    while (seq_cap < needed)
        seq_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
    if (!seq_cache)
        abort();
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* unused, offset is 1 */

    /* Grow cache up to n terms */
    if (seq_len < (size_t)n) {
        ensure_seq_cap((size_t)n);
        for (size_t i = seq_len; i < (size_t)n; ++i) {
            /* i is 0‑based index for a(i+1) */
            uint32_t prime = get_prime((idx_t)(i + 2)); /* odd primes start at index 2 */
            size_t   d     = digits_uint64((uint64_t)prime);
            seq_t    mul   = pow10((idx_t)d);             /* 10^d as seq_t */
            if (i == 0) {
                seq_cache[i] = (seq_t)prime;              /* a(1) = 3 */
            } else {
                seq_cache[i] = seq_cache[i - 1] * mul + (seq_t)prime;
            }
        }
        seq_len = (size_t)n;
    }
    return seq_cache[n - 1];
}
