/*
 * A075509  Shifts one place left under 10th-order binomial transform.
 * https://oeis.org/A075509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, eigen
 * Formula:   a(n) = Sum_{m=0..n} 10^(n-m) * S2(n,m) (S2 = Stirling2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache for the B_n,k values:  B[k] = S2(n,k) * 10^(n-k)   */
static seq_t *B = NULL;          /* dynamic array of seq_t, size = max_n+1 */
static idx_t  Bsize = 0;         /* allocated size */

/* cache of a(n) values */
static seq_t *a_cache = NULL;
static idx_t  a_cache_size = 0;  /* number of a values stored */

/* current n for which B holds the row S2(n,k)*10^(n-k) */
static idx_t current_n = 0;

/* Ensure B has at least n+1 slots and initialise new slots to 0 */
static void ensure_Bsize(idx_t n)
{
    if (n + 1 <= Bsize) return;
    size_t new_size = (size_t)(n + 1);
    seq_t *new_B = (seq_t *)realloc(B, new_size * sizeof(seq_t));
    if (!new_B) abort();
    /* zero new entries */
    for (size_t i = Bsize; i < new_size; ++i) new_B[i] = (seq_t)0;
    B = new_B;
    Bsize = new_size;
}

/* Ensure a_cache has at least n+1 slots and initialise new slots to 0 */
static void ensure_a_cache_size(idx_t n)
{
    if (n + 1 <= a_cache_size) return;
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();
    for (size_t i = a_cache_size; i < new_size; ++i) new_cache[i] = (seq_t)0;
    a_cache = new_cache;
    a_cache_size = new_size;
}

/* public function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise caches on first call */
    if (!B) {
        B = (seq_t *)calloc(1, sizeof(seq_t));
        Bsize = 1;
        B[0] = (seq_t)1;               /* B_0,0 = 1 */
    }
    if (!a_cache) {
        a_cache = (seq_t *)calloc(1, sizeof(seq_t));
        a_cache_size = 1;
        a_cache[0] = (seq_t)1;         /* a(0) = 1 */
    }
    if (current_n == 0 && B[0] != (seq_t)1) { /* ensure initial value */
        B[0] = (seq_t)1;
    }

    /* extend precomputation up to n if needed */
    if (n > current_n) {
        ensure_Bsize(n);
        ensure_a_cache_size(n);

        for (idx_t k = current_n + 1; k <= n; ++k) {
            /* compute B for new row k */
            /* loop from highest index downwards to avoid overwriting needed values */
            for (idx_t i = k; i > 0; --i) {
                seq_t oldBi   = B[i];
                seq_t oldBi_1 = B[i - 1];
                seq_t mult    = (seq_t)(10 * i);      /* 10*i */
                B[i] = oldBi * mult + oldBi_1;
            }
            B[0] = (seq_t)0;   /* S2(k,0)=0 for k>0 */

            /* compute a(k) = sum_{i=0..k} B[i] */
            seq_t sum = (seq_t)0;
            for (idx_t i = 0; i <= k; ++i) sum += B[i];
            a_cache[k] = sum;
        }
        current_n = n;
    }

    return a_cache[n];
}
