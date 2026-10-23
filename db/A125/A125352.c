/*
 * A125352  Number of base 15 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n >= 1, a(0) = 1, where A is the 15x15
 *            adjacency matrix with A[i][j] = 1 iff |i-j| <= 4 (digits 0..14).
 *            The trace of A^n counts circular n‑digit base‑15 numbers whose
 *            adjacent digits differ by at most 4. The implementation builds
 *            the powers of A incrementally using only addi...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/*  helper: multiply a seq_t by an unsigned integer (up to 32 bits) */
extern seq_t mul_small(seq_t x, unsigned int m);

/* ------------------------------------------------------------ */
/*  Build the adjacency neighbourhood lists                  */
/* ------------------------------------------------------------ */
#define N 15
#define MAX_NEIGH 9
static int neigh[N][MAX_NEIGH];
static int neigh_cnt[N];
static int neigh_init = 0;

static void init_neigh(void)
{
    if (neigh_init) return;
    for (int t = 0; t < N; ++t) {
        int cnt = 0;
        for (int j = 0; j < N; ++j) {
            if (t >= j ? t - j <= 4 : j - t <= 4) {
                neigh[t][cnt++] = j;
            }
        }
        neigh_cnt[t] = cnt;
    }
    neigh_init = 1;
}

/* ------------------------------------------------------------ */
/*  Core data structures                                       */
/* ------------------------------------------------------------ */
static seq_t M[N][N];          /* current power of A   */
static seq_t Mnew[N][N];       /* temporary matrix     */
static seq_t *cache = NULL;    /* cache of a(n) values */
static idx_t cache_len = 0;     /* first unused index   */
static size_t cache_cap = 0;    /* allocated size       */

/* ------------------------------------------------------------ */
/*  Ensure the cache has space for indices up to idx          */
/* ------------------------------------------------------------ */
static void need_cache(idx_t idx)
{
    if (idx < cache_len) return;
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }
    while (cache_len <= idx) {
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache) abort();
        }
        /* Compute the next term of the sequence */
        /*  1. Build Mnew = M * A  (using only additions)   */
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                Mnew[i][j] = 0;

        for (int t = 0; t < N; ++t) {
            for (int k = 0; k < neigh_cnt[t]; ++k) {
                int j = neigh[t][k];
                for (int i = 0; i < N; ++i) {
                    Mnew[i][j] += M[i][t];
                }
            }
        }

        /*  2. Trace of Mnew gives a(cache_len)          */
        seq_t tr = 0;
        for (int i = 0; i < N; ++i)
            tr += Mnew[i][i];
        cache[cache_len] = tr;
        ++cache_len;

        /*  3. Prepare for next iteration:  M = Mnew   */
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                M[i][j] = Mnew[i][j];
    }
}

/* ------------------------------------------------------------ */
/*  Main function                                               */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!neigh_init) init_neigh();

    /*  Initialise M = I (0‑th power of A) and cache[0] = 1  */
    if (cache_len == 0) {
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                M[i][j] = 0;
        for (int i = 0; i < N; ++i) M[i][i] = 1;
        cache_cache: ; /* dummy */
    }
    if (cache_len == 0) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;   /* a(0) = 1 */
        cache_len = 1;
    }

    need_cache(n);
    return cache[n];
}
