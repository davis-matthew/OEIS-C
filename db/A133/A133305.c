/*
 * A133305  a(n) = (1/n)*Sum_{i=0..n-1} C(n,i)*C(n,i+1)*4^i*5^(n-i), a(0) = 1.
 * https://oeis.org/A133305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A133305
   Recurrence:
     a(0) = 1, a(1) = 5
     (n+1)*a(n) = 9*(2*n-1)*a(n-1) - (n-2)*a(n-2)   for n >= 2
  The function computes a(n) for any n ≥ 0 using that recurrence
  and memoises intermediate values.  It works for 8‑, 16‑, 32‑, 64‑,
  128‑bit unsigned types and for arbitrary‑precision integer types
  as supplied by the build.  For built‑in integer types that can
  overflow during the multiplication step, the intermediate result
  is computed in unsigned __int128.  For arbitrary‑precision types
  the operator overloads perform the arithmetic safely.  The code
  follows the C11 / C++17 compatibility rules requested. */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *a_cache = NULL;
static size_t a_cache_len = 0;

/* Ensure the cache contains entries up to index n */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= a_cache_len)
        return; /* already computed */

    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
    if (!tmp)
        abort(); /* out of memory */
    a_cache = tmp;

    /* compute missing values */
    for (idx_t i = a_cache_len; i <= n; ++i)
    {
        if (i == 0)
            a_cache[0] = (seq_t)1;
        else if (i == 1)
            a_cache[1] = (seq_t)5;
        else
        {
            /* Use 128‑bit intermediate when possible to avoid overflow */
            if (sizeof(seq_t) <= sizeof(unsigned long long))
            {
                __uint128_t prod1 = ((__uint128_t)a_cache[i-1])
                                 * ((__uint128_t)(9ULL * (2*i-1)));
                __uint128_t prod2 = ((__uint128_t)a_cache[i-2])
                                 * ((__uint128_t)(i-2));
                __uint128_t num   = prod1 - prod2;
                a_cache[i] = (seq_t)(num / (i+1));
            }
            else
            {
                seq_t term1 = a_cache[i-1] * (seq_t)(9ULL * (2*i-1));
                seq_t term2 = a_cache[i-2] * (seq_t)(i-2);
                a_cache[i] = (term1 - term2) / (i+1);
            }
        }
    }
    a_cache_len = new_len;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
