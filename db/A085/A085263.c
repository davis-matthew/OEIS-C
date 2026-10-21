/*
 * A085263  Number of ways to write n as the sum of a squarefree number
 *          (A005117) and a positive square (A000290).
 * https://oeis.org/A085263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = # { k>=1 : n - k^2 is squarefree } (OEIS A085263)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function works for any n >= 1.  It lazily builds a sieve of
 * square‑free numbers up to the largest n seen so far and then counts
 * the admissible squares for each call.  This is O(√n) per call
 * (≤ 100 for n ≤ 10 000).  No external libraries or complex
 * structures are required. */

#include <stddef.h>
#include <stdlib.h>

/* static global buffers for the square‑free sieve */
static uint8_t *sf_array = NULL;   /* 1 = square‑free, 0 = not */
static idx_t  sf_size  = 0;

/* integer square‑root for idx_t (no <math.h> used) */
static idx_t isqrt_idx(idx_t v)
{
    idx_t lo = 0, hi = v, mid, r = 0;
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        uint64_t sq = (uint64_t)mid * (uint64_t)mid;
        if (sq <= v) {
            r = mid;
            lo = mid + 1;
        } else
            hi = mid - 1;
    }
    return r;
}

/* Build or grow the square‑free sieve up to limit. */
static void build_squarefree(idx_t limit)
{
    /* Allocate new array */
    uint8_t *new_arr = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    if (!new_arr) {
        /* Allocation failure – fallback to very small array */
        return;
    }

    /* Initialise all entries as square‑free (1).  0 is not used. */
    for (idx_t i = 0; i <= limit; ++i)
        new_arr[i] = 1;
    if (limit >= 0) new_arr[0] = 0;   /* 0 is never used */

    /* Mark multiples of p^2 as NOT square‑free. */
    for (idx_t p = 2; p * p <= limit; ++p) {
        idx_t p2 = p * p;
        for (idx_t m = p2; m <= limit; m += p2) {
            new_arr[m] = 0;
        }
    }

    /* Replace old array */
    if (sf_array) free(sf_array);
    sf_array = new_arr;
    sf_size  = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n=0 but safe guard */

    /* Ensure the sieve covers up to n */
    if (n > sf_size)
        build_squarefree(n);

    /* Count admissible squares */
    seq_t count = 0;
    idx_t max_k = isqrt_idx(n - 1);   /* largest k with k^2 < n */
    for (idx_t k = 1; k <= max_k; ++k) {
        idx_t k2 = k * k;
        if (k2 >= n) break;            /* safety */
        idx_t s = n - k2;              /* candidate square‑free part */
        if (sf_array[s]) count++;      /* s is square‑free */
    }
    return count;
}
