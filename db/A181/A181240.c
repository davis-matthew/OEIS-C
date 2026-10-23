/*
 * A181240  Number of nX5 binary matrices with no 2X2 circuit having pattern
 *          0101 in any orientation.
 * https://oeis.org/A181240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21*a(n-1)+9*a(n-2)-278*a(n-3)+73*a(n-4)+790*a(n-5)
 *            -662*a(n-6)+29*a(n-7)+69*a(n-8)-10*a(n-9) for n ≥ 10 Initial
 *            values: a(1)=32, a(2)=634, a(3)=13262, a(4)=275690,
 *            a(5)=5735478, a(6)=119310334, a(7)=2481942354, a(8)=51630303190,
 *            a(9)=1074033301458
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* allocation helpers that work for both C and C++ */
static seq_t* allocate_seq_array(size_t cap)
{
#if defined(__cplusplus)
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static void free_seq_array(seq_t* p)
{
#if defined(__cplusplus)
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n=0, but safe */

    /* static cache for memoisation */
    static seq_t *memo = NULL;
    static size_t memo_size = 0;          /* number of terms stored */
    static size_t memo_cap  = 0;          /* allocated capacity */

    /* initialise cache once with the first 9 terms */
    if (memo == NULL) {
        memo_cap = 32;                    /* initial capacity, > 9 */
        memo    = allocate_seq_array(memo_cap);
        seq_t init[9] = {
            (seq_t)32,
            (seq_t)634,
            (seq_t)13262,
            (seq_t)275690,
            (seq_t)5735478,
            (seq_t)119310334,
            (seq_t)2481942354,
            (seq_t)51630303190,
            (seq_t)1074033301458
        };
        for (size_t i = 0; i < 9; ++i)
            memo[i] = init[i];
        memo_size = 9;
    }

    /* extend cache up to requested n */
    while (memo_size < n) {
        /* grow the array if necessary */
        if (memo_size >= memo_cap) {
            size_t new_cap = memo_cap * 2;
            seq_t *new_mem = allocate_seq_array(new_cap);
            for (size_t i = 0; i < memo_size; ++i)
                new_mem[i] = memo[i];
            free_seq_array(memo);
            memo      = new_mem;
            memo_cap  = new_cap;
        }

        /* compute a(n) for the next index (1‑based) */
        /* at this point memo_size >= 9, so all terms used exist */
        seq_t val =
            21 * memo[memo_size-1] +
            9  * memo[memo_size-2] -
            278 * memo[memo_size-3] +
            73  * memo[memo_size-4] +
            790 * memo[memo_size-5] -
            662 * memo[memo_size-6] +
            29  * memo[memo_size-7] +
            69  * memo[memo_size-8] -
            10 * memo[memo_size-9];

        memo[memo_size++] = val;
    }

    return memo[n-1];  /* sequence is 1‑based in OEIS */
}
