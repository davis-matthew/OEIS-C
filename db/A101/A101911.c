/*
 * A101911  Binomial transform of A101910, where A101910(n) = a(A000120(n-1))
 *          for n>0 with A101910(0) = 1.
 * https://oeis.org/A101911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  eigen, nonn
 * Formula:   a(0) = 1 a(n) = 1 + Sum_{k=0}^{n-1} C(n,k) * a(A000120(n-k-1))
 *            (n>0)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------
   Memoisation utilities that work both in C and in C++17.
   For C, new[]/delete[] are not available, so we fall back to
   malloc()/realloc() and free().
   ------------------------------------------------------------ */
#include <stddef.h>
#ifdef __cplusplus
#  include <cstddef>
#endif

static seq_t *memo = NULL;      /* array of a[0 .. memo_size-1]      */
static idx_t  memo_size = 0;    /* allocated capacity                 */
static idx_t  computed   = 0;   /* highest index already computed     */

static seq_t *create_array(size_t len)
{
#ifdef __cplusplus
    return new seq_t[len];
#else
    return (seq_t*)malloc(sizeof(seq_t)*len);
#endif
}

static seq_t *grow_array(seq_t *old, size_t old_len, size_t new_len)
{
#ifdef __cplusplus
    seq_t *new_arr = new seq_t[new_len];
    for (size_t i = 0; i < old_len; ++i)
        new_arr[i] = old[i];
    delete[] old;
    return new_arr;
#else
    return (seq_t*)realloc(old, sizeof(seq_t)*new_len);
#endif
}

/* Ensure the memo array can hold index n (including n). */
static void ensure_capacity(idx_t n)
{
    if (memo_size <= n) {
        size_t new_size = memo_size ? memo_size * 2 : 1;
        while (new_size <= (size_t)n)
            new_size <<= 1;        /* double until large enough   */
        memo = grow_array(memo, memo_size, new_size);
        memo_size = new_size;
    }
}

/* ------------------------------------------------------------
   Main sequence function.
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Initialise memo with a(0)=1 on first call. */
    if (memo == NULL) {
        memo = create_array(1);
        memo[0] = (seq_t)1;
        memo_size = 1;
        computed  = 0;
    }

    if (n == 0)
        return memo[0];

    /* Expand memo/compute up to index n if required. */
    if (n > computed) {
        ensure_capacity(n);
        for (idx_t i = computed; i < n; ++i) {
            /* Compute a(i+1).  Let nn = i+1. */
            const idx_t nn = i + 1;
            seq_t sum   = (seq_t)0;
            seq_t binom = (seq_t)1;                      /* C(nn,0) */

            for (idx_t k = 0; k < nn; ++k) {
                /* A000120(nn-k-1) = popcount(nn-k-1) */
                idx_t pc = (idx_t)__builtin_popcountll((uint64_t)(nn - k - 1));
                sum += binom * memo[pc];

                /* Next binomial coefficient C(nn, k+1) */
                binom = binom * (seq_t)(nn - k) / (seq_t)(k + 1);
            }
            memo[i + 1] = sum + (seq_t)1;
        }
        computed = n;
    }

    return memo[n];
}
