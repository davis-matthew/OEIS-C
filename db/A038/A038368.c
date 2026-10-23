/*
 * A038368  n is divisible by |(product of digits) - (sum of digits)|.
 * https://oeis.org/A038368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the n‑th positive integer k such that k is divisible by
 *            |(product of digits of k) - (sum of digits of k)|
 * Range:     u8 n <= 74, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/digit_product.h"
#include "../utils/digit_sum.h"

/* Static cache of sequence values. */
static seq_t *buf        = NULL;     /* array of found terms      */
static size_t buf_cap    = 0;        /* current capacity in elements */
static size_t buf_len    = 0;        /* number of terms stored     */
static seq_t next_chk    = 1;        /* next integer to test       */

/* Ensure that the buffer can hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= buf_cap)
        return;
    size_t new_cap = buf_cap ? buf_cap << 1 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
    buf = tmp;
    buf_cap = new_cap;
}

/* Return the n‑th element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but avoid UB */
    while (buf_len < (size_t)n) {
        ++next_chk;
        seq_t prod = digit_product(next_chk);
        seq_t sum  = digit_sum(next_chk);
        seq_t diff = prod > sum ? prod - sum : sum - prod;
        if (diff == 0) /* skip division by zero case */
            continue;
        if (next_chk % diff == 0) {
            ensure_capacity(buf_len + 1);
            buf[buf_len++] = next_chk;
        }
    }
    return buf[(size_t)n - 1];
}
