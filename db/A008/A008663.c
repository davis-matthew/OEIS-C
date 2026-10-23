/*
 * A008663  Theta series of direct sum of 2 copies of f.c.c. lattice.
 * https://oeis.org/A008663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Theta series of direct sum of 2 copies of f.c.c. lattice  
   a(n) = Σ_{k=0}^{2n} b(k)·b(2n-k)  
   where b(k) = #{(x,y,z)∈ℤ³ : x+y+z even, x²+y²+z² = k}. */
#include <stdlib.h>
#include <stdint.h>

static seq_t *b_cache = NULL;   /* b(k) for k < b_size */
static size_t b_size = 0;       /* number of b entries stored */
static seq_t *a_cache = NULL;   /* a(n) for n < a_size */
static size_t a_size = 0;       /* number of a entries stored */

/* compute all b(k) for k < new_b_size (new_b_size = 2*max_n+1) */
static void recompute_b(size_t new_b_size)
{
    seq_t *new_b = (seq_t *)malloc(new_b_size * sizeof(seq_t));
    if (!new_b) return; /* allocation failure: leave caches unchanged */

    for (size_t i = 0; i < new_b_size; ++i) new_b[i] = 0;

    /* maximal coordinate needed */
    int R = 0;
    while ((idx_t)(R + 1) * (R + 1) <= (idx_t)(new_b_size - 1)) ++R;

    for (int x = -R; x <= R; ++x) {
        int xsq = x * x;
        for (int y = -R; y <= R; ++y) {
            int ysq = y * y;
            int xy_sq = xsq + ysq;
            for (int z = -R; z <= R; ++z) {
                if (((x + y + z) & 1) == 0) {          /* parity even */
                    int sumsq = xy_sq + z * z;
                    if (sumsq < (int)new_b_size) {
                        new_b[(size_t)sumsq] += 1;
                    }
                }
            }
        }
    }

    if (b_cache) free(b_cache);
    b_cache = new_b;
    b_size = new_b_size;
}

/* recompute a(n) for n < new_a_size (new_a_size = max_n+1) */
static void recompute_a(size_t new_a_size)
{
    seq_t *new_a = (seq_t *)malloc(new_a_size * sizeof(seq_t));
    if (!new_a) return; /* allocation failure: leave caches unchanged */

    for (size_t i = 0; i < new_a_size; ++i) new_a[i] = 0;

    for (size_t n = 0; n < new_a_size; ++n) {
        seq_t acc = 0;
        size_t limit = 2 * n;            /* sum over k = 0 .. 2n */
        for (size_t k = 0; k <= limit; ++k) {
            acc += b_cache[k] * b_cache[limit - k];
        }
        new_a[n] = acc;
    }

    if (a_cache) free(a_cache);
    a_cache = new_a;
    a_size = new_a_size;
}

/* ensure caches contain entries up to n */
static void ensure_cache(idx_t n)
{
    if ((idx_t)a_size > n) return; /* already computed */

    /* compute b up to 2*n */
    size_t required_b_size = 2 * (size_t)n + 1;
    if (b_size < required_b_size) {
        recompute_b(required_b_size);
    }

    /* compute a up to n */
    size_t required_a_size = (size_t)n + 1;
    if (a_size < required_a_size) {
        recompute_a(required_a_size);
    }
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[(size_t)n];
}
