/*
 * A216193  Number of inequivalent 5-variable monotone Boolean functions with
 *          n mincuts.
 * https://oeis.org/A216193
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute by enumerating all 5‑variable monotone Boolean functions (7581
   antichains of B5), canonicalising under the 120 permutations of the
   variables, and counting the unique classes having a given number of
   mincuts (size of the antichain).  */
/* OEIS: A216193 – Number of inequivalent 5‑variable monotone Boolean
   functions with n mincuts.  Offset: 2   */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* 5-variable permutations – 120! */
#define NUM_PERMS 120
static int perms[NUM_PERMS][5];
static int perm_count = 0;

/* generate all permutations of 5 indices */
static void gen_perms(int idx, int cur[5], bool used[5]) {
    if (idx == 5) {
        for (int i = 0; i < 5; ++i) perms[perm_count][i] = cur[i];
        ++perm_count;
        return;
    }
    for (int i = 0; i < 5; ++i) {
        if (used[i]) continue;
        used[i] = true;
        cur[idx] = i;
        gen_perms(idx + 1, cur, used);
        used[i] = false;
    }
}

/* antichains of the 32‑element Boolean lattice B5 */
#define MAX_ANTICH 8000           /* safe upper bound, 7581 actual */
static uint32_t antichains[MAX_ANTICH];
static int antichain_count = 0;

/* recursive construction of all antichains */
static void rec_antichain(int start, uint32_t mask, int chosen[32], int chosen_sz) {
    for (int e = start; e < 32; ++e) {
        bool ok = true;
        for (int k = 0; k < chosen_sz; ++k) {
            int a = chosen[k];
            if (((e & a) == a) || ((e & a) == e)) { ok = false; break; }
        }
        if (ok) {
            chosen[chosen_sz] = e;
            rec_antichain(e + 1, mask | (1U << e), chosen, chosen_sz + 1);
        }
    }
    /* store current antichain mask */
    if (antichain_count < MAX_ANTICH) {
        antichains[antichain_count++] = mask;
    }
}

/* canonicalise an antichain mask under all variable permutations */
static uint32_t canonical_mask(uint32_t mask) {
    uint32_t best = 0xFFFFFFFFU;
    for (int p = 0; p < perm_count; ++p) {
        uint32_t m = 0;
        for (int i = 0; i < 32; ++i) {
            if (mask & (1U << i)) {
                /* element i (a 5‑bit subset) becomes permuted */
                int r = 0;
                for (int b = 0; b < 5; ++b) {
                    if ( (i >> b) & 1 ) r |= 1U << perms[p][b];
                }
                m |= 1U << r;
            }
        }
        if (m < best) best = m;
    }
    return best;
}

/* qsort comparator for uint32_t */
static int cmp_uint32(const void *a, const void *b) {
    uint32_t va = *(const uint32_t *)a;
    uint32_t vb = *(const uint32_t *)b;
    return (va < vb) - (va > vb);
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n < 2 || n > 10) return 0;      /* out of range, return 0 */

    static int initialized = 0;
    static seq_t results[11];           /* results[2..10] used */

    if (!initialized) {
        /* generate permutations */
        int cur[5];
        bool used[5] = {0};
        gen_perms(0, cur, used);

        /* generate all antichains */
        int chosen[32];
        rec_antichain(0, 0U, chosen, 0);

        /* canonicalise all */
        uint32_t *cans = (uint32_t *)malloc(sizeof(uint32_t) * antichain_count);
        for (int i = 0; i < antichain_count; ++i) {
            cans[i] = canonical_mask(antichains[i]);
        }

        /* sort canonicals to group equal classes */
        qsort(cans, antichain_count, sizeof(uint32_t), cmp_uint32);

        /* count unique classes per size (popcount) */
        memset(results, 0, sizeof(results));
        for (int i = 0; i < antichain_count; ++i) {
            if (i > 0 && cans[i] == cans[i - 1]) continue; /* duplicate */
            int sz = __builtin_popcount(cans[i]);           /* size of antichain */
            if (sz >= 2 && sz <= 10) {
                results[sz] = (seq_t)(results[sz] + 1);
            }
        }

        free(cans);
        initialized = 1;
    }

    /* results is indexed by actual n (2..10) */
    return results[(int)n];
}
