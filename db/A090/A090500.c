/*
 * A090500  Sums described in A090499.
 * https://oeis.org/A090500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=A090499(n)..A090499(n+1)-1} A000005(i)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ---------- Pre‑compute divisor counts ---------------------------------- */
static uint16_t *divisor_counts = NULL;   /* divisor counts, 1‑based */
static size_t    divisor_size   = 0;       /* allocated size */

/* Initialise the divisor count table up to a fixed limit. */
static void
init_divisor_counts(void)
{
    /* For n ≈ 10000 the required index is well below five million. */
    const size_t LIMIT = 5000000;
    if (divisor_counts != NULL)
        return;     /* already initialised */

    divisor_size = LIMIT + 1;          /* index 0 unused */
    divisor_counts = (uint16_t*)calloc(divisor_size, sizeof(uint16_t));
    if (divisor_counts == NULL)
        abort();                       /* allocation failed */

    /* sieve divisor counts */
    for (size_t i = 1; i <= LIMIT; ++i)
        for (size_t j = i; j <= LIMIT; j += i)
            ++divisor_counts[j];
}

/* ---------- Cache for a(n) values --------------------------------------- */
static seq_t *a_cache   = NULL;   /* array of computed a(n) values */
static size_t cache_cap = 0;      /* current capacity of a_cache */
static size_t cache_len = 0;      /* highest index computed (1‑based) */

/* Ensure that a_cache can hold indices up to 'need' (0‑based). */
static void
ensure_cache_cap(idx_t need)
{
    if ((size_t)need + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while ((size_t)need + 1 > new_cap)
        new_cap *= 2;

    seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort();                     /* allocation failed */
    a_cache   = tmp;
    cache_cap = new_cap;
}

/* ---------- Block generation state -------------------------------------- */
static idx_t  next_i          = 1;   /* next integer to process           */
static seq_t  prev_a          = 0;   /* a(k) for the last finished block */
static size_t computed_blocks = 0;   /* number of blocks computed so far  */

/* ---------- a(n) implementation ---------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n=0, return safe value */

    init_divisor_counts();

    /* Expand cache as needed */
    if (cache_len <= (size_t)n) {
        ensure_cache_cap(n);
        /* Compute blocks until we reach n */
        while (computed_blocks < (size_t)n) {
            seq_t sum = 0;
            /* Accumulate divisor counts until sum > prev_a */
            while (sum <= prev_a) {
                if ((size_t)next_i >= divisor_size) {
                    /* Should not occur for n up to ~10000 */
                    abort();
                }
                sum += (seq_t)divisor_counts[next_i];
                ++next_i;
            }
            /* Store the new term */
            ++computed_blocks;
            a_cache[computed_blocks - 1] = sum;  /* zero‑based index */
            prev_a = sum;
        }
        cache_len = computed_blocks;           /* highest index now computed */
    }

    return a_cache[n - 1];   /* zero‑based lookup */
}
