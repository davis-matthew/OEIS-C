/*
 * A025104  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (composite numbers), t = (odd natural numbers).
 * https://oeis.org/A025104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} s(i) * (2*(n-i+1)+1)
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           s(i) = i-th composite number (4,6,8,9,10,…),   */
/*           t(j) = 2*j+1  (odd numbers starting at 3)         */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* store the composite numbers as uint64_t indices */
static idx_t *composite_idx = NULL;
static size_t composite_len = 0;   /* number of composites currently cached */
static size_t composite_cap = 0;   /* allocated capacity */
static idx_t  next_candidate  = 4; /* next integer to test for composite */

/* Ensure that composite_idx[0 .. k-1] contains the first k composite numbers */
static void ensure_composites(idx_t k)
{
    if (k <= composite_len)
        return;

    /* grow allocation if necessary */
    if (composite_cap < (size_t)k) {
        size_t new_cap = composite_cap ? composite_cap : 64;
        while (new_cap < (size_t)k)
            new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(composite_idx, new_cap * sizeof(idx_t));
        composite_idx = tmp;
        composite_cap = new_cap;
    }

    /* find and store missing composites */
    while (composite_len < (size_t)k) {
        while (is_prime(next_candidate))
            ++next_candidate;
        composite_idx[composite_len++] = next_candidate;
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n = 0, safe */

    idx_t k = (n + 1) / 2;               /* floor((n+1)/2) */
    ensure_composites(k);                /* make sure we have the first k composites */

    seq_t sum = 0;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t s = (seq_t)composite_idx[i - 1];   /* i-th composite number */
        idx_t pos = n - i + 1;                  /* position for t() */
        seq_t t = ((seq_t)pos * 2) + 1;         /* 2*pos + 1   */
        sum += s * t;
    }
    return sum;
}
