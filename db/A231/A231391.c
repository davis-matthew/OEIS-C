/*
 * A231391  Number of (n+1) X (3+1) 0..2 arrays with no element unequal to a
 *          strict majority of its horizontal, diagonal and antidiagonal
 *          neighbors, with values 0..2 introduced in row major order.
 * https://oeis.org/A231391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the linear recurrence * * a(n) = 4*a(n-1) -
 *            8*a(n-3) - 8*a(n-4) + 7*a(n-5) * +12*a(n-6) +23*a(n-7)
 *            -42*a(n-8) -16*a(n-9) * +81*a(n-10) -14*a(n-11) -52*a(n-12)
 *            +15*a(n-13) * - 2*a(n-14) -17*a(n-15) + 4*a(n-16) +10*a(n-17) *
 *            + 4*a(n-18) * * with the initial values * * a(1)=14, a(2)...
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* First 18 terms of the sequence (index 0 unused), stored as uint64_t.
   For arbitrary‑precision builds we cast these values to seq_t when
   needed. */
static const unsigned long long init_uint[19] = {
    0ULL,   /* unused */
    14ULL, 38ULL, 100ULL, 272ULL, 740ULL,
    2061ULL, 5834ULL, 16521ULL, 46969ULL, 133864ULL,
    382377ULL, 1093837ULL, 3133209ULL, 8984580ULL,
    25782696ULL, 74034161ULL, 212690121ULL, 611260183ULL
};

/* Coefficients of the recurrence, indexed by the offset from the term
   to the current a(n).  coeff[0] is unused. */
static const int coeff[19] = {
    0,    /* unused */
    4,    /* a(n-1)  */
    0,    /* a(n-2)  */
   -8,   /* a(n-3)  */
   -8,   /* a(n-4)  */
    7,   /* a(n-5)  */
   12,   /* a(n-6)  */
   23,   /* a(n-7)  */
  -42,   /* a(n-8)  */
  -16,   /* a(n-9)  */
   81,   /* a(n-10) */
  -14,   /* a(n-11) */
  -52,   /* a(n-12) */
   15,   /* a(n-13) */
   -2,   /* a(n-14) */
  -17,   /* a(n-15) */
    4,   /* a(n-16) */
   10,   /* a(n-17) */
    4   /* a(n-18) */
};

/* Memoisation cache:  cache[i] holds a(i) for i >= 1.
   cache[0] is unused.  The array is grown on demand. */
static seq_t *cache = NULL;     /* 0‑based, cache[0] unused */
static size_t cache_cap = 0;    /* number of allocated slots */
static size_t cache_len = 0;    /* current highest index cached (>=0) */

seq_t a(idx_t n)
{
    if (n < 1) /* out of range – behaviour unspecified, return 0 */
        return 0;

    /* Make sure the cache array is large enough.  Cast n to size_t
       for the allocation size – safe for n up to 10 000 and far beyond. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                 /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms that are not yet cached. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 18) {
            /* Initial terms are known. */
            cache[(size_t)i] = (seq_t)init_uint[(size_t)i];
        } else {
            /* Recurrence requires terms up to 18 steps back.  We
               accumulate positive and negative contributions
               separately to avoid underflow on unsigned types. */
            seq_t pos = 0;
            seq_t neg = 0;
            for (int k = 1; k <= 18; ++k) {
                int c = coeff[k];
                if (!c)
                    continue;
                /* cache[i-k] is already computed because i>=19. */
                const seq_t term = cache[(size_t)(i - k)];
                if (c > 0) {
                    pos += term * (seq_t)c;
                } else {          /* c < 0 */
                    neg += term * (seq_t)(-c);
                }
            }
            cache[(size_t)i] = pos - neg;   /* pos >= neg by definition */
        }
    }
    cache_len = (size_t)n;
    return cache[(size_t)n];
}
