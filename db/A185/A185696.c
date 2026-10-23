/*
 * A185696  Eigensequence for the Gould sequence triangle (with general term
 *          A001316(n-k) if k<=n, 0 otherwise).
 * https://oeis.org/A185696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n≥1: * a(n) = Σ_{k=0}^{n-1} 2^{popcount(k)} *
 *            a(n-1-k)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static caches, maintained across calls */
static seq_t *a_cache   = NULL;   /* a[0 .. size-1] */
static idx_t   a_size   = 0;      /* number of elements stored */

static uint64_t *q_cache = NULL; /* q[0 .. size-1] where q[k]=2^{popcount(k)} */
static idx_t    q_size   = 0;     /* number of q elements stored */

/* helper: ensure q_cache has entries up to n */
static void ensure_q(idx_t n)
{
    if (q_size <= n) {
        size_t new_size = (size_t)n + 1;
        uint64_t *new_q = (uint64_t *)realloc(q_cache, new_size * sizeof(uint64_t));
        if (!new_q) abort();
        q_cache = new_q;
        for (idx_t i = q_size; i <= n; ++i) {
            uint64_t pc = __builtin_popcountll((uint64_t)i);
            q_cache[i] = 1ull << pc;   /* 2^{popcount(i)} */
        }
        q_size = new_size;
    }
}

/* helper: ensure a_cache has entries up to n */
static void ensure_a(idx_t n)
{
    if (a_size <= n) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_a = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!new_a) abort();
        a_cache = new_a;
        /* initialise newly added entries */
        for (idx_t i = a_size; i <= n; ++i) {
            if (i == 0) {
                a_cache[0] = (seq_t)1;
            } else {
                seq_t sum = (seq_t)0;
                ensure_q(i - 1);   /* need q[0..i-1] */
                for (idx_t k = 0; k < i; ++k) {
                    sum = sum + ((seq_t)q_cache[k]) * a_cache[i - 1 - k];
                }
                a_cache[i] = sum;
            }
        }
        a_size = new_size;
    }
}

/* main entry for the sequence */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* ensure the first element is present for future calls */
        ensure_a(0);
        return a_cache[0];
    }
    ensure_a(n);
    return a_cache[n];
}
