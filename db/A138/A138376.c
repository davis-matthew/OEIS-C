/*
 * A138376  a(n+1) = abs[ a(n) + (-1)^(n+1) * Sum_of_digits_of(n+1)], with
 *          a(0)=0.
 * https://oeis.org/A138376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(0) = 0; for n ≥ 0, a(n + 1) = abs( a(n) + (-1)^(n + 1) *
 *            sum_of_digits(n + 1) )
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Needed for dynamic allocation and memory copy */
#include <stdlib.h>
#include <string.h>

/* Sum of decimal digits of an idx_t value */
static unsigned int digit_sum_idx(idx_t x)
{
    unsigned int sum = 0;
    while (x)
    {
        sum += (unsigned int)(x % 10);
        x /= 10;
    }
    return sum;
}

/* Static cache of computed sequence values */
static seq_t *a_cache = NULL;
static size_t a_len = 0;      /* number of values currently stored (a_cache[0..a_len-1]) */
static size_t a_cap = 0;      /* allocated capacity of a_cache */

/* Compute a(n) for any n ≥ 0, reusing cached values */
seq_t a(idx_t n)
{
    /* Initialise cache with a(0) if empty */
    if (!a_cache)
    {
        a_cap = 1;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        if (!a_cache) abort();          /* unrecoverable on out‑of‑memory */
        a_cache[0] = (seq_t)0;
        a_len = 1;
    }

    /* Grow the cache if necessary to reach index n */
    if (n >= a_len)
    {
        /* Determine new capacity (double until large enough) */
        size_t new_cap = a_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= n) new_cap <<= 1;

        if (new_cap > a_cap)
        {
            seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* unrecoverable on out‑of‑memory */
            a_cache = tmp;
            a_cap = new_cap;
        }

        /* Compute new sequence values up to n */
        for (idx_t i = a_len; i <= n; ++i)
        {
            unsigned int sum = digit_sum_idx(i);
            int sign = (i & 1) ? -1 : 1;                   /* (-1)^i : +1 for even, -1 for odd */
            long long temp = (long long)a_cache[i - 1]      /* a(i-1) as signed */
                            + (long long)sign * (long long)sum;
            seq_t val = temp >= 0 ? (seq_t)temp : (seq_t)(-temp);
            a_cache[i] = val;
        }

        a_len = n + 1;      /* now a_cache[0..a_len-1] are valid */
    }

    return a_cache[n];
}
