/*
 * A173589  Integers whose binary representation contains exactly three 1's,
 *          no two 1's being adjacent.
 * https://oeis.org/A173589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A173589: Integers whose binary representation contains
   exactly three 1's, no two 1's being adjacent.
   This function generates all such numbers with 64‑bit positions,
   sorts them, and returns the (n‑1)th entry. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Local comparator for qsort */
static int cmp_seq_t(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Cache of precomputed values */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Build the cache once.  We consider bit positions 0..63.
   The number of admissible triples is C(64-3+1,3)=37820,
   which exceeds the required limit of about 10 000.  */
static void build_cache(void)
{
    size_t cap = 4096;
    cache = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!cache) return;           /* allocation failure → leave cache empty  */

    for (int i = 0; i <= 59; ++i) {          /* i <= 59 ensures room for j,k */
        for (int j = i + 2; j <= 61; ++j) {  /* j must be ≥ i+2 */
            for (int k = j + 2; k <= 63; ++k) {  /* k ≥ j+2 */
                seq_t val = ((seq_t)1ULL << i) |
                           ((seq_t)1ULL << j) |
                           ((seq_t)1ULL << k);
                if (cache_len == cap) {
                    cap <<= 1;
                    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                    if (!cache) return;
                }
                cache[cache_len++] = val;
            }
        }
    }
    qsort(cache, cache_len, sizeof(seq_t), cmp_seq_t);
}

/* External function required by the harness */
seq_t a(idx_t n)
{
    if (cache == NULL) build_cache();

    /* n is 1‑based; guard against out‑of‑range requests */
    if (n == 0 || cache_len < (size_t)n) return (seq_t)0;
    return cache[(size_t)n - 1];
}
