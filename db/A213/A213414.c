/*
 * A213414  Half the number of n X 4 binary arrays with no 3 X 3 submatrix
 *          formed with any three rows and columns equal to J-I.
 * https://oeis.org/A213414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 51*a(n-1) - 1075*a(n-2) + 11985*a(n-3) - 74524*a(n-4) +
 *            245004*a(n-5) - 332640*a(n-6) with initial values: a(1)=8,
 *            a(2)=128, a(3)=1970, a(4)=28952, a(5)=407498, a(6)=5524088
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include required headers */
#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, free */

static seq_t *seq_cache = NULL;     /* cached sequence values */
static size_t  seq_cap   = 0;        /* capacity of seq_cache */
static idx_t   seq_len   = 0;        /* number of values stored */

/* Ensure the cache has room for at least 'need' elements (1‑based indexing) */
static void ensure_capacity(idx_t need)
{
    if (need <= (idx_t)seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < (size_t)need)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (seq_cache) {
        for (size_t i = 0; i < seq_len; ++i)
            new_cache[i] = seq_cache[i];
        free(seq_cache);
    }
    seq_cache = new_cache;
    seq_cap   = new_cap;
}

/* Return the n-th term of sequence A213414 (n >= 1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, but safe guard */

    /* First time initialization of the cache */
    if (seq_len == 0) {
        /* Ensure at least 6 elements */
        ensure_capacity(6);
        seq_cache[0] = (seq_t)8;         /* a(1) */
        seq_cache[1] = (seq_t)128;       /* a(2) */
        seq_cache[2] = (seq_t)1970;      /* a(3) */
        seq_cache[3] = (seq_t)28952;     /* a(4) */
        seq_cache[4] = (seq_t)407498;    /* a(5) */
        seq_cache[5] = (seq_t)5524088;   /* a(6) */
        seq_len = 6;
    }

    /* Extend the cache up to index n */
    if (n > seq_len) {
        ensure_capacity(n);
        for (idx_t i = seq_len; i < n; ++i) {
            /* Recurrence:  a(i) = 51*a(i-1) - 1075*a(i-2) + 11985*a(i-3)
                           - 74524*a(i-4) + 245004*a(i-5) - 332640*a(i-6) */
            seq_t pos = (51  * seq_cache[i-1]   ) +
                        (11985* seq_cache[i-3]   ) +
                        (245004*seq_cache[i-5]   );
            seq_t neg = (1075*seq_cache[i-2]   ) +
                        (74524*seq_cache[i-4]   ) +
                        (332640*seq_cache[i-6]  );
            seq_cache[i] = pos - neg;   /* pos > neg for all valid n */
        }
        seq_len = n;
    }

    return seq_cache[n-1];
}
