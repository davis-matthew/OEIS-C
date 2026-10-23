/*
 * A130043  a(1)=1. a(n) = number of earlier terms which are coprime to the
 *          largest odd divisor of n.
 * https://oeis.org/A130043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of earlier terms that are coprime to the largest
 *            odd divisor of n
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *values = NULL;   /* a(1), a(2), … stored sequentially */
static idx_t values_cap = 0;   /* current allocated capacity */
static idx_t values_len = 0;   /* number of terms stored */

/* Ensure capacity for at least need elements. */
static void grow_if_needed(idx_t need)
{
    if (need <= values_cap)
        return;
    idx_t new_cap = values_cap ? values_cap : 16;
    while (new_cap < need)
        new_cap <<= 1; /* double until sufficient */

    /* realloc returns void*, cast to seq_t* */
    seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
    values = tmp;
    values_cap = new_cap;
}

/* Return the largest odd divisor of an unsigned 64‑bit integer. */
static seq_t odd_part_seq(idx_t x)
{
    while ((x & 1ULL) == 0)
        x >>= 1;
    return (seq_t)x;
}

/* Compute the sequence up to index n (inclusive). */
static void compute_up_to(idx_t n)
{
    if (n <= values_len)
        return;
    for (idx_t k = values_len + 1; k <= n; ++k) {
        if (k == 1) {                 /* a(1) = 1 */
            grow_if_needed(values_len + 1);
            values[values_len] = (seq_t)1;
            ++values_len;
            continue;
        }
        seq_t om = odd_part_seq(k);
        idx_t cnt = 0;
        for (idx_t i = 0; i < values_len; ++i) {
            seq_t g = gcd(values[i], om);
            if (g == 1)
                ++cnt;
        }
        grow_if_needed(values_len + 1);
        values[values_len] = (seq_t)cnt;
        ++values_len;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined for n < 1 */
    compute_up_to(n);
    return values[n - 1];
}
