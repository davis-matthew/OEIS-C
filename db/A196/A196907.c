/*
 * A196907  Number of nX3 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 3,1,2,0,4 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A196907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A196907:
   a(n) = number of n×3 arrays over {0,1,2,3,4}
   where element x has exactly (3,1,2,0,4) neighbors equal to x.
   The sequence satisfies the 18‑term linear recurrence

   a(n) = 2*a(n-1)+2*a(n-2)+16*a(n-3)+16*a(n-4)+5*a(n-5)
          -19*a(n-6)-53*a(n-7)-52*a(n-8)+15*a(n-9)+41*a(n-10)+24*a(n-11)
          -8*a(n-12)-28*a(n-13)-10*a(n-14)+7*a(n-15)+5*a(n-16)+4*a(n-17)
          -2*a(n-18)

   with initial terms
   a(1)=3, a(2)=13, a(3)=50, a(4)=193, a(5)=731, a(6)=2843,
   a(7)=10970, a(8)=42213, a(9)=162766, a(10)=627709,
   a(11)=2419707, a(12)=9327886, a(13)=35961933, a(14)=138640724,
   a(15)=534481526, a(16)=2060527603, a(17)=7943732911, a(18)=30624562306.
*/
#include <stddef.h>
#include <stdlib.h>

/* Initial 18 terms (1‑based). */
static const seq_t init[18] = {
    (seq_t)3,            /* a(1) */
    (seq_t)13,           /* a(2) */
    (seq_t)50,           /* a(3) */
    (seq_t)193,          /* a(4) */
    (seq_t)731,          /* a(5) */
    (seq_t)2843,         /* a(6) */
    (seq_t)10970,        /* a(7) */
    (seq_t)42213,        /* a(8) */
    (seq_t)162766,       /* a(9) */
    (seq_t)627709,       /* a(10) */
    (seq_t)2419707,      /* a(11) */
    (seq_t)9327886,      /* a(12) */
    (seq_t)35961933,     /* a(13) */
    (seq_t)138640724,    /* a(14) */
    (seq_t)534481526,    /* a(15) */
    (seq_t)2060527603,   /* a(16) */
    (seq_t)7943732911,   /* a(17) */
    (seq_t)30624562306   /* a(18) */
};

/* Coefficients for a(n‑1)…a(n‑18). */
static const int coeff[18] = {
    2,   2,  16,  16,   5, -19, -53, -52,  15,  41,  24,
   -8, -28, -10,   7,   5,   4,  -2
};

/* Compute a term when the previous 18 terms are known. */
static inline seq_t compute_value(const seq_t *cache, size_t k)
{
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        /* Built‑in integer types – use signed __int128 to avoid overflow. */
        signed __int128 sum = 0;
        for (int i = 0; i < 18; ++i) {
            sum += (signed __int128)cache[k - i - 1] * coeff[i];
        }
        return (seq_t)sum;
    } else {
        /* Arbitrary-precision or larger types. */
        seq_t pos = 0, neg = 0;
        for (int i = 0; i < 18; ++i) {
            seq_t term = cache[k - i - 1];
            int c = coeff[i];
            if (c >= 0) {
                pos += (seq_t)c * term;
            } else {
                neg += (seq_t)(-c) * term;
            }
        }
        return pos - neg;   /* pos ≥ neg for valid terms. */
    }
}

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of cached terms */

    /* Initialise cache with known terms. */
    if (cache_len == 0) {
        cache_len = 18;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (size_t i = 0; i < 18; ++i) cache[i] = init[i];
    }

    /* Return value if already cached. */
    if ((size_t)n <= cache_len) return cache[n - 1];

    /* Grow cache to include term n. */
    size_t new_len = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;          /* allocation failed */
    cache = tmp;
    for (size_t k = cache_len; k < new_len; ++k) {
        cache[k] = compute_value(cache, k);
    }
    cache_len = new_len;
    return cache[n - 1];
}
