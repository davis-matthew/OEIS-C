/*
 * A120231  Split-floor-multiplier sequence (SFMS) using multipliers 2/3 and
 *          3/2. (SFMS is defined at A120229.).
 * https://oeis.org/A120231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(2n/3) if that value is positive and not used
 *            before, otherwise a(n) = floor(3n/2). The function stores
 *            results in a static cache so that calls can be made in arbitrary
 *            order
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static storage for the sequence and for the set of values already seen. */
static seq_t *seq_cache          = NULL;   /* cache of a(n), 1‑based */
static size_t cache_cap          = 0;      /* allocated size of seq_cache */
static idx_t  computed_upto      = 0;      /* maximum n that has been computed */

static bool   *used              = NULL;   /* bitmap of values already used */
static size_t used_cap           = 0;      /* allocated size of used array */

/* Ensure that the cache is grown to hold at least indices up to 'n'.  Then
   compute the missing terms from (computed_upto+1) up to 'n'. */
static void
ensure_cache(idx_t n)
{
    if (n <= computed_upto) return;

    /* Grow seq_cache if necessary (allocate at least n+1 slots for 1‑based index). */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t k = computed_upto + 1; k <= n; ++k) {
        /* Candidate from the first rule: floor(2*k/3) */
        seq_t cand1 = ((seq_t)k * 2) / 3;
        bool use_cand1 = false;

        if (cand1 > 0) {
            /* Make sure the 'used' array can index cand1. */
            if ((size_t)cand1 >= used_cap) {
                size_t new_used = used_cap ? used_cap * 2 : 16;
                while (new_used <= (size_t)cand1) new_used *= 2;
                bool *new_arr = (bool *)realloc(used, new_used * sizeof(bool));
                if (new_arr) {
                    if (new_used > used_cap) {
                        for (size_t i = used_cap; i < new_used; ++i)
                            new_arr[i] = false;
                    }
                    used = new_arr;
                    used_cap = new_used;
                }
            }
            if (cand1 <= SIZE_MAX && !used[(size_t)cand1])
                use_cand1 = true;
        }

        seq_t val;
        if (use_cand1)
            val = cand1;
        else
            val = ((seq_t)k * 3) / 2;   /* second rule */

        /* Store the computed value. */
        seq_cache[k] = val;

        /* Ensure the 'used' array covers 'val' and mark it. */
        if ((size_t)val >= used_cap) {
            size_t new_used = used_cap ? used_cap * 2 : 16;
            while (new_used <= (size_t)val) new_used *= 2;
            bool *new_arr = (bool *)realloc(used, new_used * sizeof(bool));
            if (new_arr) {
                if (new_used > used_cap) {
                    for (size_t i = used_cap; i < new_used; ++i)
                        new_arr[i] = false;
                }
                used = new_arr;
                used_cap = new_used;
            }
        }
        used[(size_t)val] = true;
    }

    computed_upto = n;
}

/* Public function for the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, but safe */
    ensure_cache(n);
    return seq_cache[n];
}
