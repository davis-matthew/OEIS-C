/*
 * A049912  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = 2*n
 *          - 3 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1, a(2) = 2, and a(3) = 4.
 * https://oeis.org/A049912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=4; for n>=4, a(n)= (a(1)+...+a(n-1)) - a(m)
 *            where m = 2*n-3-2^(p+1) and p is the largest integer with 2^p <
 *            n-1 <= 2^(p+1). The implementation memoises all values up to the
 *            greatest requested index and keeps a running cumulative sum
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *a_cache = NULL;     /* stores a(i), 0‑based index for a(i+1) */
static seq_t *cum_cache = NULL;  /* cumulative sums: cum_cache[i] = a(1)+...+a(i+1) */
static size_t cache_len = 0;     /* number of terms stored so far */
static size_t cache_cap = 0;     /* allocated capacity */

/* Helper: return floor(log2(x)) for x>0.  */
static inline int floor_log2_uint64(idx_t x)
{
    /* x is guaranteed non‑zero for the call sites. */
    return 63 - __builtin_clzll((unsigned long long)x);
}

seq_t a(idx_t n)
{
    /* Ensure we have the first three terms. */
    if (cache_len == 0) {
        /* guarantee capacity at least 4 */
        size_t init_cap = 4;
        a_cache = (seq_t *)realloc(a_cache, init_cap * sizeof(seq_t));
        cum_cache = (seq_t *)realloc(cum_cache, init_cap * sizeof(seq_t));
        cache_cap = init_cap;

        a_cache[0] = (seq_t)1;          /* a(1)   */
        cum_cache[0] = a_cache[0];      /* cumulative sum = 1          */
        a_cache[1] = (seq_t)2;          /* a(2)   */
        cum_cache[1] = cum_cache[0] + a_cache[1]; /* =3          */
        a_cache[2] = (seq_t)4;          /* a(3)   */
        cum_cache[2] = cum_cache[1] + a_cache[2]; /* =7          */
        cache_len = 3;
    }

    /* Expand storage if necessary. */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 4;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        cum_cache = (seq_t *)realloc(cum_cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute successive terms until we reach the requested n. */
    while (cache_len < n) {
        idx_t cur = cache_len + 1;  /* current n (1‑based) */

        /* Find p: largest integer with 2^p < cur-1 <= 2^(p+1). */
        idx_t n_minus1 = cur - 1;
        int p = floor_log2_uint64(n_minus1);
        if ((n_minus1 & (n_minus1 - 1)) == 0)    /* n-1 is a power of two */
            p--;

        /* Compute m = 2*n-3-2^(p+1). */
        idx_t pow2p1 = ((idx_t)1) << (p + 1);
        idx_t m = 2 * cur - 3 - pow2p1;   /* m is always >= 1 and < cur */

        /* sum_{k=1}^{n-1} a(k)  */
        seq_t sum_prev = cum_cache[cache_len - 1];
        seq_t a_m = a_cache[(size_t)(m - 1)];   /* m < cur, safe index */

        seq_t a_cur = sum_prev - a_m;

        a_cache[cache_len] = a_cur;
        cum_cache[cache_len] = sum_prev + a_cur;
        cache_len++;
    }

    return a_cache[(size_t)(n - 1)];
}
