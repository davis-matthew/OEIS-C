/*
 * A049936  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = 2*n
 *          - 3 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = a(2) = 1 and a(3) = 2.
 * https://oeis.org/A049936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049936
   a(1) = 1, a(2) = 1, a(3) = 2
   For n >= 4:
      a(n) = (a(1)+...+a(n-1)) + a(m)
      where m = 2*n - 3 - 2^{p+1}
      and 2^p < n-1 <= 2^{p+1}   (p = ⌊log₂(n-2)⌋)
   The routine memoises values and works for any order of calls. */

#include <stdlib.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* a[1..max_n] */
    static seq_t *pref    = NULL;   /* prefix sums, pref[i]=a[1]+...+a[i] */
    static size_t cap     = 0;      /* allocated size (index 0 unused) */
    static idx_t  max_n   = 0;      /* highest index computed so far */

    if (n < 1) return (seq_t)0;     /* undefined behaviour for 0 */

    /* Ensure capacity for indices up to n */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (cap ? cap * 2 : 4);
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;

        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        seq_t *new_pref  = (seq_t *)realloc(pref,  new_cap * sizeof(seq_t));
        if (!new_cache || !new_pref) return (seq_t)0; /* allocation failure */

        cache = new_cache;
        pref  = new_pref;
        cap   = new_cap;
    }

    /* Compute terms up to n (if not already done) */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)          val = (seq_t)1;
        else if (i == 2)      val = (seq_t)1;
        else if (i == 3)      val = (seq_t)2;
        else {
            /* Determine 2^p where 2^p < i-1 <= 2^{p+1} */
            idx_t x   = i - 2;                           /* n-2 >= 1 for i>=4 */
            unsigned p = floor_log2_uint64(x);           /* p = ⌊log₂(x)⌋ */
            idx_t pow = (idx_t)1 << p;                   /* 2^p */
            idx_t e   = pow << 1;                        /* 2^{p+1} */

            idx_t m = (i << 1) - 3 - e;                  /* 2*i - 3 - 2^{p+1} */
            val = pref[i-1] + cache[m];
        }
        cache[i] = val;
        pref[i]  = (i == 1) ? val : pref[i-1] + val;
    }
    max_n = n;
    return cache[n];
}
