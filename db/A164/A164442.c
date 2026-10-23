/*
 * A164442  Number of binary strings of length n with no substrings equal to
 *          0000 0111 or 1001.
 * https://oeis.org/A164442
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 49, u64 n <= 101,
 *            u128 n <= 205, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: DP over last three bits with forbidden 4‑bit substrings
   0000, 0111, 1001.  The number of valid binary strings of length n
   is derived by extending all valid strings of length n−1 and
   discarding any extension that would create a forbidden pattern. */

#include <stdlib.h>
#ifdef __cplusplus
#include <cstddef>
#endif

/* Allocate an array of seq_t.  Works with both fundamental integer
   types (malloc) and arbitrary‑precision class types (operator new). */
static seq_t *seq_alloc(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

/* Resize a previously allocated seq_t array.  For C++ the array
   is recreated and elements are copied. */
static seq_t *seq_resize(seq_t *oldp, size_t oldn, size_t newn)
{
#ifdef __cplusplus
    seq_t *newp = new seq_t[newn];
    size_t m = oldn < newn ? oldn : newn;
    for (size_t i = 0; i < m; ++i) newp[i] = oldp[i];
    delete [] oldp;
    return newp;
#else
    return (seq_t *)realloc(oldp, newn * sizeof(seq_t));
#endif
}

/* Free an allocated seq_t array. */
static void seq_free(seq_t *p)
{
#ifdef __cplusplus
    delete [] p;
#else
    free(p);
#endif
}

static seq_t *a_cache = NULL;   /* a_cache[n] = a(n) */
static size_t cache_size = 0;   /* number of allocated entries */
static idx_t  computed_len = 0; /* highest n already computed */

static seq_t dp_curr[8]; /* counts for strings having the current last 3 bits */
static seq_t dp_next[8]; /* next state counts */

/* Ensure a_cache[n] is computed for all n up to the requested n. */
static void ensure_computed(idx_t n)
{
    if (n < 4) return; /* values below offset are undefined */

    if (a_cache == NULL)
    {
        size_t init = n + 1;
        if (init < 1) init = 1;
        a_cache = seq_alloc(init);
        cache_size = init;

        for (int i = 0; i < 8; ++i) dp_curr[i] = 0;
        dp_curr[0] = 1; /* empty string at length 0 */
        computed_len = 0;
    }

    if (n <= computed_len) return;

    if ((size_t)(n + 1) > cache_size)
    {
        size_t new_size = (size_t)(n + 1);
        a_cache = seq_resize(a_cache, cache_size, new_size);
        cache_size = new_size;
    }

    for (idx_t len = computed_len; len < n; ++len)
    {
        for (int i = 0; i < 8; ++i) dp_next[i] = 0;

        for (int s = 0; s < 8; ++s)
        {
            seq_t val = dp_curr[s];
            if (val == 0) continue;

            for (int b = 0; b < 2; ++b)
            {
                int new_state = ((s << 1) | b) & 7; /* keep last 3 bits */

                if (len >= 3)
                {
                    int pat = ((s << 1) | b); /* 4‑bit pattern */
                    if (pat == 0 || pat == 7 || pat == 9) /* 0000,0111,1001 */
                        continue;
                }

                dp_next[new_state] += val;
            }
        }

        for (int i = 0; i < 8; ++i) dp_curr[i] = dp_next[i];

        seq_t sum = 0;
        for (int i = 0; i < 8; ++i) sum += dp_curr[i];
        a_cache[len + 1] = sum;
    }

    computed_len = n;
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;
    ensure_computed(n);
    return a_cache[n];
}
