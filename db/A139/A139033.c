/*
 * A139033  a(1)=1; at n>=2, a(n) = least square > a(n-1) such that sum
 *          a(1)+...+a(n) is a prime number.
 * https://oeis.org/A139033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 215, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   floor_sqrt, isPrimeGeneric, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A139033:  a(1)=1; for n≥2, a(n)=least square > a(n‑1)  
   such that a(1)+…+a(n) is prime.  This is equivalent to  
   a(n)=A137326(n)^2.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *vals   = NULL;          /* sequence values, 1‑based → vals[0]=a(1) */
    static size_t cap    = 0;             /* malloced capacity */
    static seq_t running_sum = 0;         /* Σ_{k=1}^{i-1} a(k) for current i */
    static idx_t next_compute = 1;        /* next index to compute (1‑based) */

    /* already computed? */
    if (n < next_compute)
        return vals[n - 1];

    /* enlarge storage if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap < (size_t)n) new_cap <<= 1;
        if (cap == 0)
            vals = seq_alloc(new_cap);
        else
            vals = seq_realloc(vals, cap, new_cap);
        if (!vals) abort();           /* out of memory */
        cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = next_compute; i <= n; ++i) {
        seq_t val;
        if (i == 1) {                 /* first term */
            val = (seq_t)1;
            running_sum = val;
        } else {
            seq_t prev      = vals[i - 2];   /* a(i-1) */
            seq_t prev_sum  = running_sum;   /* Σ_{k=1}^{i-1} a(k) */
            seq_t k         = floor_sqrt(prev) + 1;   /* smallest integer with k² > prev */
            while (1) {
                seq_t cand      = k * k;               /* candidate square */
                seq_t new_sum   = prev_sum + cand;     /* cumulative sum if cand chosen */
                if (isPrimeGeneric(new_sum)) {
                    val = cand;
                    running_sum = new_sum;
                    break;
                }
                ++k;
            }
        }
        vals[i - 1] = val;
    }

    next_compute = n + 1;
    return vals[n - 1];
}
