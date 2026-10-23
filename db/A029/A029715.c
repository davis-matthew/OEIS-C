/*
 * A029715  a(n) = Sum_{k divides 2^n} S(k), where S is the Kempner function
 *          A002034.
 * https://oeis.org/A029715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k | 2^n} S(k) where S is the Kempner function
 * Range:     u8 n <= 19, u16 n <= 357, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache to store computed a(n) values.  They are built
   incrementally; a_cache[0] = 1 (since S(1) = 1).  For n > 0 we
   compute a(n) = a(n-1) + S(2^n).  The Kempner value for 2^d is
   the smallest integer m such that m! is divisible by 2^d.
   For powers of two, this is equivalent to finding minimal m
   with m - popcount(m) >= d.  We update m incrementally,
   keeping track of the current value of m and v = m - popcount(m). */

static seq_t *a_cache = NULL;          /* array of a values */
static size_t  a_capacity = 0;         /* allocated size of a_cache */
static idx_t   a_max = (idx_t)-1;       /* largest n for which a_cache[n] is computed */

/* Variables for computing successive Kempner values S(2^d) */
static seq_t  m_cur = 1;   /* current m in the iteration */
static seq_t  v_cur = 0;   /* current v = m_cur - popcount(m_cur) */
static idx_t  d_cur = 0;   /* current exponent d for which S(2^d) is known */

static bool initialized = false;

seq_t a(idx_t n)
{
    /* initialisation on first call */
    if (!initialized) {
        a_capacity = 1;
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        if (a_cache == NULL) abort();        /* allocation failed */
        a_cache[0] = 1;                      /* a(0) = S(1) = 1 */
        a_max = 0;
        /* m_cur and v_cur already set to 1 and 0, satisfying v_cur >= d_cur (0) */
        initialized = true;
    }

    /* if n already computed, return cached value */
    if (n <= a_max) return a_cache[n];

    /* Ensure cache has enough space for indices up to n */
    if ((idx_t)a_capacity <= n) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();            /* reallocation failed */
        a_cache = tmp;
        a_capacity = new_cap;
    }

    /* Compute values from a_max+1 up to n */
    for (idx_t d = a_max + 1; d <= n; ++d) {
        /* Increment m_cur until v_cur >= d (the new exponent) */
        while (v_cur < (seq_t)d) {
            /* Count trailing ones in the current m_cur */
            idx_t t = 0;
            seq_t tmp = m_cur;
            while ((tmp & 1) == 1) {
                ++t;
                tmp >>= 1;
            }
            m_cur += 1;
            v_cur += t;            /* v(m+1) = v(m) + trailing_ones(m) */
        }
        seq_t s_d = m_cur;          /* S(2^d) = current m_cur */
        a_cache[d] = a_cache[d - 1] + s_d;  /* update running sum */
    }

    a_max = n;
    return a_cache[n];
}
