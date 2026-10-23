/*
 * A153394  G.f.: A(x) = F(x*G(x)^2)^3 where F(x) = G(x*F(x)) = 1 + x*F(x)^3
 *          is the g.f. of A001764 and G(x) = F(x/G(x)) = 1 + x*G(x)^2 is the
 *          g.f. of A000108 (Catalan).
 * https://oeis.org/A153394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(3k+3,k)/(k+1) * C(2n,n-k) * k / n (for
 *            n>0, a(0)=1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

/* Global caches for the sequence and the auxiliary values */
static seq_t *A_cache = NULL;  /* a(n) values          */
static seq_t *D_cache = NULL;  /* d(k) = C(3k+3,k)/(k+1) values */
static idx_t  cache_max = 0;   /* maximum n cached     */

/* Helper to allocate or enlarge the cache arrays */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max) return;           /* already have enough space */

    /* Allocate or grow the arrays */
    seq_t *A_new = (seq_t *)realloc(A_cache, (n + 1) * sizeof(seq_t));
    seq_t *D_new = (seq_t *)realloc(D_cache, (n + 1) * sizeof(seq_t));
    assert(A_new && D_new);               /* failure is fatal */
    A_cache = A_new;
    D_cache = D_new;

    /* Initialise the first values */
    if (cache_max == 0) {
        A_cache[0] = (seq_t)1;            /* a(0) = 1   */
        D_cache[0] = (seq_t)1;            /* d(0) = 1   */
    }

    /* Compute d(k) for k = cache_max+1 .. n using a recurrence that
       keeps intermediate results within 128‑bit before truncating to
       64‑bit (when seq_t is an integer type).  For arbitrary‑precision
       seq_t the cast from uint64_t to seq_t is legal. */
    unsigned __int128 B_prev;        /* B_k = d_k * (k+1) */
    if (cache_max == 0)
        B_prev = 1;                  /* d0 * 1 = 1   */
    else
        B_prev = (unsigned __int128)D_cache[cache_max] * (cache_max + 1);

    for (idx_t k = cache_max + 1; k <= n; ++k) {
        /* Compute B_k = d_k*(k+1) from B_{k-1} */
        unsigned __int128 num = (unsigned __int128)(3 * k + 3) *
                                (unsigned __int128)(3 * k + 2) *
                                (unsigned __int128)(3 * k + 1);
        unsigned __int128 den = (unsigned __int128)k *
                                (unsigned __int128)(2 * k + 3) *
                                (unsigned __int128)(2 * k + 2);
        unsigned __int128 B = B_prev * num / den;
        /* d_k = B / (k+1)  (exact division) */
        uint64_t d_k_val = (uint64_t)(B / (k + 1));
        D_cache[k] = (seq_t)d_k_val;
        B_prev = B;
    }

    /* Compute a(n) for n = cache_max+1 .. n */
    for (idx_t m = cache_max + 1; m <= n; ++m) {
        seq_t sum = 0;
        seq_t cur = (seq_t)1;          /* C(2m,0)     */

        for (idx_t r = 0; r < m; ++r) {
            idx_t k = m - r;            /* 1 <= k <= m  */
            seq_t term = D_cache[k];
            term = term * (seq_t)k;
            term = term * cur;
            sum += term;

            /* Update cur to C(2m, r+1) */
            if (r < m - 1) {
                seq_t mult = (seq_t)(2 * m - r);
                seq_t div  = (seq_t)(r + 1);
                cur = cur * mult / div;
            }
        }

        /* a(m) = sum / m  (division is exact) */
        A_cache[m] = sum / (seq_t)m;
    }

    cache_max = n;
}

/* Public interface required by the tests */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    ensure_cache(n);
    return A_cache[n];
}
