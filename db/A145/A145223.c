/*
 * A145223  a(n) is the number of odd permutations (of an n-set) with exactly
 *          2 fixed points.
 * https://oeis.org/A145223
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n,2) * o_{n-2} , where o_k is the number of odd
 *            derangements of a k‑set. o_k is computed from derangement
 *            numbers D_k and the closed form for the alternating sum f_k =
 *            Σ_{π∈D_k} sign(π) = (-1)^k * (1-k) : o_k = (D_k - f_k)/2. The
 *            derangements D_k satisfy D_0=1, D_1=0, D_k = (k-1)*(D_{k-1...
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cached values of derangements D_k and odd derangements o_k. */
static seq_t *D_cache = NULL;
static seq_t *o_cache = NULL;
static idx_t cache_size = 0;   /* number of entries in the caches (≥ 2) */

/* Ensure that caches contain entries up to index 'need' (inclusive). */
static void
ensure_cache(idx_t need)
{
    if (need < cache_size)
        return;                                     /* already sufficient */

    idx_t newsize = (need + 1 > 2) ? need + 1 : 2;   /* at least 2 entries */
    seq_t *newD = (seq_t *)realloc(D_cache, newsize * sizeof(seq_t));
    seq_t *newo = (seq_t *)realloc(o_cache, newsize * sizeof(seq_t));

    if (!newD || !newo) {
        /* on failure abort – environment guarantees enough memory */
        if (newD) free(newD);
        if (newo) free(newo);
        exit(EXIT_FAILURE);
    }

    if (cache_size == 0) {             /* first allocation – initialise */
        newD[0] = (seq_t)1;
        newD[1] = (seq_t)0;
        newo[0] = (seq_t)0;            /* o_0 = 0 */
        newo[1] = (seq_t)0;            /* o_1 = 0 */
    }

    /* Compute any new entries beyond the previous cache size. */
    idx_t start = (cache_size > 1) ? cache_size : 2;
    for (idx_t k = start; k < newsize; ++k) {
        /* Derangement recurrence: D_k = (k-1)*(D_{k-1}+D_{k-2}). */
        seq_t dk1 = newD[k - 1];
        seq_t dk2 = newD[k - 2];
        newD[k] = (seq_t)(k - 1) * (dk1 + dk2);

        /* f_k = (-1)^k * (1 - k). */
        seq_t fk;
        if ((k & 1) == 0)               /* k even */
            fk = (seq_t)1 - (seq_t)k;
        else                            /* k odd  */
            fk = (seq_t)k - (seq_t)1;

        /* o_k = (D_k - f_k) / 2. */
        newo[k] = (newD[k] - fk) / (seq_t)2;
    }

    /* Commit new arrays. */
    D_cache = newD;
    o_cache = newo;
    cache_size = newsize;
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    if (n < 2)                /* sequence is defined for n ≥ 2 */
        return (seq_t)0;

    idx_t m = n - 2;          /* we need odd derangements of m elements */

    ensure_cache(m);          /* guarantee o_cache[m] exists */

    seq_t odd_der = o_cache[m];

    seq_t nseq = (seq_t)n;
    seq_t binom = nseq * (nseq - (seq_t)1) / (seq_t)2;  /* C(n,2) */

    return binom * odd_der;
}
