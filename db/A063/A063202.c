/*
 * A063202  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          22 ).
 * https://oeis.org/A063202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A063202 ‑ Dimension of the space of weight 2n cuspidal newforms for Γ0(22).
   The generating function is:
        G(x) = x^2 (3 + 5x + 2x^2 – x^3 + x^4) / ((1+x)(1+x+x^2)(x-1)^2).
   From this we obtain the linear recurrence for n ≥ 7
        a(n) = a(n‑2) + a(n‑3) – a(n‑5),
   with initial values
        a(1)=0, a(2)=3, a(3)=5, a(4)=5, a(5)=7, a(6)=11. */

#include <stdlib.h>
#include <stdint.h>

/* Memoisation array (1‑based index).  The array includes a dummy
   entry at index 0 to simplify recurrence handling. */
static seq_t *cache   = NULL;   /* allocated array of terms      */
static size_t cache_len = 0;    /* highest index that currently
                                   contains a computed value     */
static size_t cache_cap = 0;    /* allocated capacity (elements) */

/* Ensure that the cache can hold at least indices [0 … needed‑1]. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the first six values of the sequence.  Called automatically
   on the first request. */
static void init_initial(void)
{
    if (cache_len >= 6) return;          /* already initialised */
    ensure_cache(7);                      /* need indices 0…6 */
    cache[0] = 0;                         /* dummy (not used)   */
    cache[1] = 0;                          /* a(1) = 0          */
    cache[2] = 3;                          /* a(2) = 3          */
    cache[3] = 5;                          /* a(3) = 5          */
    cache[4] = 5;                          /* a(4) = 5          */
    cache[5] = 7;                          /* a(5) = 7          */
    cache[6] = 11;                         /* a(6) = 11         */
    cache_len = 6;                         /* highest cached index */
}

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Handle out‑of‑range indices gracefully.  The specification
       only guarantees correctness for n ≥ 1. */
    if (n < 1) return 0;

    init_initial();                      /* make sure a(1)…a(6) exist */

    size_t idx = (size_t)n;
    if (idx <= cache_len)                /* already computed */
        return cache[idx];

    ensure_cache(idx + 1);                /* need space up to this index */

    /* Compute terms from cache_len+1 up to idx using the recurrence.
       The first 7 terms (indices 0…6) are already known. */
    for (size_t i = cache_len + 1; i <= idx; ++i) {
        /* For i ≥ 7 the recurrence is homogeneous:
              a(i) = a(i‑2) + a(i‑3) – a(i‑5). */
        cache[i] = cache[i - 2] + cache[i - 3] - cache[i - 5];
    }
    cache_len = idx;
    return cache[idx];
}
