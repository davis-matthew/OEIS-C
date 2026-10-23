/*
 * A008121  Coordination sequence T1 for Zeolite Code JBW.
 * https://oeis.org/A008121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 170, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A008121 Coordination sequence T1 for Zeolite Code JBW
   Recurrence (n ≥ 16):
      a(n) = -(-1*a(n-1) + 2*a(n-2) - 2*a(n-3) + 1*a(n-4)
                - 3*a(n-5) + 2*a(n-6) - 4*a(n-7) + 4*a(n-8)
                - 2*a(n-9) + 3*a(n-10) - 1*a(n-11) + 2*a(n-12)
                - 2*a(n-13) + 1*a(n-14) - 1*a(n-15))
   Initial values:
      a(0)=1, a(1)=4, a(2)=10, a(3)=21, a(4)=39, a(5)=61,
      a(6)=81, a(7)=107, a(8)=148, a(9)=192, a(10)=228,
      a(11)=266, a(12)=324, a(13)=393, a(14)=449, a(15)=503.
 */

#include <stddef.h>
#include <stdlib.h>

/* Base values a(0) … a(15). */
static const seq_t base_seq[16] = {
    (seq_t)1,  (seq_t)4,  (seq_t)10, (seq_t)21, (seq_t)39,
    (seq_t)61, (seq_t)81, (seq_t)107, (seq_t)148, (seq_t)192,
    (seq_t)228, (seq_t)266, (seq_t)324, (seq_t)393, (seq_t)449,
    (seq_t)503
};

/* Recurrence coefficients c1 … c15. */
static const int coeff[15] = {
    -1,  2, -2,  1, -3,  2, -4,  4, -2,  3, -1,  2, -2,  1, -1
};

seq_t a(idx_t n)
{
    /* Cache for previously computed terms.  The cache grows as needed. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of elements computed so far */

    size_t idx = (size_t)n;
    if (idx >= cache_len) {
        size_t new_len = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;       /* allocation failed */
        cache = tmp;

        /* Initialise base terms that are not yet stored. */
        size_t i;
        for (i = cache_len; i < new_len && i < 16; ++i)
            cache[i] = base_seq[i];

        /* Compute missing terms using the recurrence. */
        for (i = cache_len; i <= idx; ++i) {
            if (i < 16)                     /* already set above            */
                continue;
            seq_t v = 0;
            for (int k = 1; k <= 15; ++k) {
                int c = coeff[k - 1];
                seq_t t = cache[i - k];
                if (c > 0)
                    v -= (seq_t)c * t;     /* -c * term  */
                else
                    v += (seq_t)(-c) * t;  /* +(-c) * term */
            }
            cache[i] = v;
        }
        cache_len = new_len;
    }
    return cache[idx];
}
