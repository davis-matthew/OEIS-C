/*
 * A125929  Sprague-Grundy values for octal game .7077.
 * https://oeis.org/A125929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the Sprague–Grundy sequence of the octal game .7077:
   Let G(0) = 0.  
   For n ≥ 1, let S(n) be the set of Grundy values reachable in one move:
      • For each allowed removal k (k = 1, 3, 4):
          – If n = k, the move leaves no heaps → add 0.
          – If n > k, the move leaves a single heap of size n‑k → add G(n‑k).
          – If n > k, the move may split the remaining heap into two heaps,  
            of sizes a and (n‑k)‑a with 0 ≤ a ≤ n‑k → add G(a) ⊕ G((n‑k)‑a).
   Then G(n) = mex(S(n)).  */

/* ------------------------------------------------------------ */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static cache for Grundy values */
static seq_t *G = NULL;
static size_t G_size = 0;  /* number of stored values (G[0] … G[G_size-1]) */

/* Static array for marking seen Grundy values while computing mex */
static uint64_t *seen_mark = NULL;   /* stamp for each Grundy value */
static size_t seen_cap = 0;          /* current capacity of seen_mark */
static uint64_t current_stamp = 1;   /* stamp value increased at each mex */

/* ------------------------------------------------------------ */
/* Ensure that G has space for indices up to n (inclusive). */
static void ensure_G(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (G_size >= need) return;

    seq_t *tmp = (seq_t *)realloc(G, need * sizeof(seq_t));
    if (!tmp) abort();
    /* initialise new cells to 0 */
    for (size_t i = G_size; i < need; ++i) tmp[i] = (seq_t)0;
    G = tmp;
    G_size = need;
}

/* Ensure that seen_mark can address indices up to at least 'need' */
static void ensure_seen_cap(size_t need)
{
    if (seen_cap >= need) return;
    size_t new_cap = seen_cap ? seen_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;

    uint64_t *tmp = (uint64_t *)realloc(seen_mark, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    for (size_t i = seen_cap; i < new_cap; ++i) tmp[i] = 0;
    seen_mark = tmp;
    seen_cap = new_cap;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Remember where we start computing */
    size_t start = G_size;
    ensure_G(n);

    /* The allowed removals for .7077: k = 1, 3, 4 (all with bits 0111) */
    const idx_t moves[] = {1, 3, 4};
    const int nmoves = 3;

    for (size_t idx = start; idx <= (size_t)n; ++idx)
    {
        /* new stamp for this position */
        ++current_stamp;

        /* Enumerate reachable Grundy values */
        for (int m = 0; m < nmoves; ++m)
        {
            idx_t k = moves[m];
            if ((size_t)k > idx) continue;                /* not enough stones */

            idx_t rem = (idx_t)(idx - k);                 /* remaining after removal */

            if (rem == 0) {                               /* remove all stones */
                /* bit 1 (value 1) is set in all allowed digits, so add 0 */
                if (0 < seen_cap) seen_mark[0] = current_stamp;
            } else {                                       /* rem > 0 */
                /* leave the remainder as a single heap (bit 2 is set) */
                seq_t g_single = G[(size_t)rem];
                if (g_single < seen_cap) seen_mark[(size_t)g_single] = current_stamp;

                /* split the remainder into two heaps (bit 3 is set) */
                for (idx_t i = 0; i <= rem; ++i)
                {
                    seq_t g_split = G[(size_t)i] ^ G[(size_t)(rem - i)];
                    if (g_split < seen_cap) seen_mark[(size_t)g_split] = current_stamp;
                }
            }
        }

        /* Compute mex */
        idx_t mex = 0;
        while (true)
        {
            if (mex >= seen_cap) {                        /* extend seen_mark */
                ensure_seen_cap(mex + 1);
            }
            if (seen_mark[mex] != current_stamp) break;    /* unavailable Grundy */
            ++mex;
        }
        G[(size_t)idx] = mex;
    }

    return G[(size_t)n];
}
