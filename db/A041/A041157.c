/*
 * A041157  Denominators of continued fraction convergents to sqrt(88).
 * https://oeis.org/A041157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041157: Denominators of continued fraction convergents to sqrt(88)
   Recurrence: a(n) = 394*a(n-6) - a(n-12)
   Base values: a(0) = 1, a(1) = 2, a(2) = 3, a(3) = 5, a(4) = 8, a(5) = 21,
                a(6) = 386, a(7) = 793, a(8) = 1179, a(9) = 1972,
                a(10) = 3151, a(11) = 8274 */
#include <stddef.h>

#define A041157_MAX_CACHE 20000   /* enough for n up to roughly 20000 */

static seq_t cache[A041157_MAX_CACHE];
static size_t cache_len = 0;

/* Return the n‑th term of OEIS A041157 (denominators of continued fraction convergents to sqrt(88)). */
seq_t a(idx_t n)
{
    /* Initialise base values on first call */
    if (cache_len == 0) {
        cache[0]  =  (seq_t)1;   /* a(0) */
        cache[1]  =  (seq_t)2;   /* a(1) */
        cache[2]  =  (seq_t)3;   /* a(2) */
        cache[3]  =  (seq_t)5;   /* a(3) */
        cache[4]  =  (seq_t)8;   /* a(4) */
        cache[5]  =  (seq_t)21;  /* a(5) */
        cache[6]  =  (seq_t)386; /* a(6) */
        cache[7]  =  (seq_t)793; /* a(7) */
        cache[8]  =  (seq_t)1179;/* a(8) */
        cache[9]  =  (seq_t)1972;/* a(9) */
        cache[10] =  (seq_t)3151;/* a(10) */
        cache[11] =  (seq_t)8274;/* a(11) */
        cache_len = 12;
    }

    /* If requested index is already cached, return it directly */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Ensure we have enough space for the requested index */
    /* (The caller is expected to use n values that do not exceed A041157_MAX_CACHE-1) */
    if ((size_t)n >= A041157_MAX_CACHE) {
        /* In a C++ build with an arbitrary-precision seq_t, we could allocate
           a larger array here.  For the purpose of this generic implementation,
           we simply return zero for out‑of‑range requests to avoid undefined
           behaviour.  Adjust the limit as needed for your build. */
        return (seq_t)0;
    }

    /* Compute missing terms up to n */
    while (cache_len <= (size_t)n) {
        /* Recurrence: a(i) = 394 * a(i-6) - a(i-12) */
        cache[cache_len] = (seq_t)394 * cache[cache_len - 6] - cache[cache_len - 12];
        ++cache_len;
    }

    return cache[n];
}
