/*
 * A053281  Coefficients of the '10th-order' mock theta function phi(q).
 * https://oeis.org/A053281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 102, u32 n <= 340, u64 n <= 1204,
 *            u128 n <= 4455, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A053281 – coefficients of the 10th‑order mock theta function φ(q)
    φ(q) = Σ_{n≥0} q^(n(n+1)/2) / ((1−q)(1−q³)…(1−q^(2n+1)))
    a(n) is the coefficient of qⁿ in this expansion.
    Formula: a(n) = Σ_{k≥0} p_odd,≤2k+1 ( n − k(k+1)/2 ),
    where p_odd,≤L(m) counts partitions of m into odd parts not exceeding L. */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the build system. */
/* For C++ builds we allocate objects with new/delete,
   for C builds we use malloc/calloc/free. */
#if defined(__cplusplus)
# define ALLOC_ARRAY(T, N) new T[(size_t)(N)]
# define FREE_ARRAY(P)    delete [] (P)
#else
# define ALLOC_ARRAY(T, N) ((T*)calloc((size_t)(N), sizeof(T)))
# define FREE_ARRAY(P)    (free((void*)(P)))
#endif

/* cache of previously computed a(n) values */
static seq_t *a_cache = NULL;          /* array a[0..max_cached]          */
static idx_t  max_cached = (idx_t)-1;  /* sentinel: cache empty           */

/* Compute a[0..N] and store in a_cache.  N is inclusive. */
static void compute_up_to(idx_t N)
{
    /* already have a[0..max_cached] ? */
    if (max_cached != (idx_t)-1 && N <= max_cached)
        return;

    /* allocate new cache and helper array for partition counts */
    size_t sz = (size_t)(N + 1);
    seq_t *new_a = ALLOC_ARRAY(seq_t, sz);
    seq_t *dp    = ALLOC_ARRAY(seq_t, sz);
    if (!new_a || !dp) {          /* allocation failure – terminate */
        exit(1);
    }

    dp[0] = 1;                     /* one way to partition 0      */

    /* iterate over k = 0,1,2,…, adding odd part (2k+1) each time */
    for (idx_t k = 0; ; ++k) {
        idx_t Tidx = k * (k + 1) / 2;         /* triangular number */
        if (Tidx > N)
            break;                           /* further contributions would be zero */

        /* add new odd part 2k+1 to the partition counts */
        idx_t odd = 2 * k + 1;
        for (idx_t m = odd; m <= N; ++m)
            dp[m] += dp[m - odd];

        /* add the contribution of this k to all a[n] with n >= Tidx */
        seq_t Tval = (seq_t)Tidx;
        for (idx_t m = Tidx; m <= N; ++m)
            new_a[m] += dp[m - Tidx];
    }

    /* replace the old cache with the new one */
    if (a_cache)
        FREE_ARRAY(a_cache);          /* free old cache */
    a_cache = new_a;
    FREE_ARRAY(dp);                   /* dp array no longer needed */
    max_cached = N;
}

/* external function required by the build system */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return a_cache[n];
}
