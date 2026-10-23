/*
 * A102828  "True already", base 2, start 1: a(n) is the least integer such
 *          that the sequence up to a(n-1) written in base 2 contains
 *          floor(a(n)/2) copies of the digit a(n) % 2, with a(0) = 1.
 * https://oeis.org/A102828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(0)=1. For n>0, let c0 and c1 be the numbers of 0‑bits and
 *            1‑bits in the binary concatenation of a(0)…a(n−1). Then a(n) is
 *            the smallest integer x>a(n−1) with floor(x/2)=c_{x%2};
 *            equivalently a(n)=min{2*c0, 2*c1+1} that exceeds a(n−1)
 * Range:     u8 n <= 31, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   popcnt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/popcnt.h"

/* length of binary representation of x (number of bits) */
static seq_t bitlen(seq_t x)
{
    seq_t len = 0;
    while (x) { len++; x >>= 1; }
    return len;
}

seq_t a(idx_t n)
{
    static seq_t *values = NULL;
    static size_t alloc_size = 0;
    static idx_t max_index = (idx_t)-1;   /* not yet initialized */
    static seq_t c0 = 0, c1 = 0;          /* counts of 0‑bits and 1‑bits */

    /* initialisation on first call */
    if (max_index == (idx_t)-1) {
        alloc_size = 1;
        values = (seq_t *)malloc(alloc_size * sizeof(seq_t));
        if (!values) exit(1);
        values[0] = (seq_t)1;               /* a(0) */
        /* update counts with a(0)=1 */
        seq_t len = bitlen(values[0]);
        seq_t ones = popcnt(values[0]);
        seq_t zeros = len - ones;
        c0 = zeros;
        c1 = ones;
        max_index = 0;
    }

    /* return immediately if we already know the term */
    if (n <= max_index) return values[n];

    /* enlarge storage if necessary */
    if ((size_t)(n + 1) > alloc_size) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(values, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        values = tmp;
        alloc_size = new_size;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_index + 1; i <= n; ++i) {
        seq_t prev = values[i - 1];
        seq_t cand_odd  = 2 * c1 + 1;   /* x odd → floor(x/2)=c1 */
        seq_t cand_even = 2 * c0;       /* x even → floor(x/2)=c0 */
        seq_t next = 0;

        bool odd_ok  = cand_odd  > prev;
        bool even_ok = cand_even > prev;

        if (odd_ok && even_ok)
            next = (cand_odd < cand_even) ? cand_odd : cand_even;
        else if (even_ok) next = cand_even;
        else if (odd_ok)  next = cand_odd;
        else next = prev + 1;          /* safety, should never happen */

        values[i] = next;

        /* update counts with new term */
        seq_t len   = bitlen(next);
        seq_t ones  = popcnt(next);
        seq_t zeros = len - ones;
        c1 += ones;
        c0 += zeros;
        max_index = i;
    }

    return values[n];
}
