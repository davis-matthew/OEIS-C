/*
 * A085886  Let r and s be such that r + s = n; a(n) = minimum value of phi(r)
 *          + phi(s).
 * https://oeis.org/A085886
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min φ(r)+φ(n−r) over all positive integers r,s with
 *            r+s=n, offset 2
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t* phi_cache = NULL;
static seq_t* a_cache   = NULL;
static idx_t  cache_max = 1;  /* biggest n for which caches are valid */

/* rebuild the φ and a tables up to (and including) n */
static void rebuild_cache(idx_t n)
{
    idx_t  N   = n + 1;          /* array size */
    seq_t* new_phi = (seq_t*)calloc(N, sizeof(seq_t));
    seq_t* new_a   = (seq_t*)calloc(N, sizeof(seq_t));

    if (!new_phi || !new_a) {    /* allocation failed – abort */
        exit(1);
    }

    for (idx_t i = 0; i < N; ++i) new_phi[i] = (seq_t)i;

    for (idx_t i = 2; i < N; ++i) {
        if (new_phi[i] == (seq_t)i) {          /* prime i found */
            for (idx_t j = i; j < N; j += i) {
                new_phi[j] -= new_phi[j] / (seq_t)i;
            }
        }
    }

    /* a[n] for n>=2 */
    new_a[0] = 0;
    new_a[1] = 0;
    for (idx_t m = 2; m < N; ++m) {
        seq_t best = new_phi[1] + new_phi[m - 1];
        for (idx_t r = 2; r <= m / 2; ++r) {
            seq_t sum = new_phi[r] + new_phi[m - r];
            if (sum < best) best = sum;
        }
        new_a[m] = best;
    }

    free(phi_cache);
    free(a_cache);
    phi_cache = new_phi;
    a_cache   = new_a;
    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* undefined behaviour for offset <2, return 0 */

    if (n > cache_max) rebuild_cache(n);
    return a_cache[n];
}
