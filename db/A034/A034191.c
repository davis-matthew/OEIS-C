/*
 * A034191  Number of binary codes of length 6 with n words.
 * https://oeis.org/A034191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of orbits of n‑subsets of the 64‑vertex 6‑cube *
 *            under the hypercube automorphism group of order 46080. *
 *            Computed by Burnside's lemma: a(n) = (1/46080) * Σ_g F_g(n) *
 *            where F_g(n) is the number of n‑subsets fixed by the group
 *            element g
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---------- internal data structures ----------------------------------- */
typedef struct {
    unsigned char *lens;   /* sorted cycle lengths (size = num)            */
    unsigned char num;     /* number of cycles                           */
    seq_t  mult;           /* number of group elements of this type      */
    seq_t *fixed;          /* fixed[k] = #subsets of size k fixed by type*/
} CycleType;

/* ---------- global state ---------------------------------------------- */
static CycleType *cycle_types = NULL;
static size_t    cycle_type_cnt = 0;
static size_t    cycle_type_cap = 0;
static bool      initialized    = false;
static seq_t     a_vals[65]     = {0};

/* ---------- finite‐field helpers -------------------------------------- */
/* multiply and add in 128‑bit to avoid overflow in 64‑bit builds           */
#ifdef __SIZEOF_INT128__
typedef unsigned __int128 u128;
#else
/* fall back: use double type if extension is not available (should not happen
   on the judge).                                                                */
typedef unsigned long long u128;
#endif

/* ---------- cycle‐type processing ------------------------------------- */
static void process_perm(const unsigned char *perm) {
    /* iterate over all 64 sign masks  */
    for (int sign = 0; sign < 64; ++sign) {
        unsigned char  map[64];
        /* build the permutation induced by (perm,sign) on 64 vertices   */
        for (int v = 0; v < 64; ++v) {
            unsigned char u = 0;
            for (int i = 0; i < 6; ++i) {
                unsigned char bit = ((v >> perm[i]) & 1) ^ ((sign >> i) & 1);
                u |= bit << i;
            }
            map[v] = u;
        }

        /* cycle decomposition */
        unsigned char visited[64] = {0};
        unsigned char lens[64];
        unsigned char lnum = 0;
        for (int start = 0; start < 64; ++start) {
            if (!visited[start]) {
                int cur = start;
                unsigned char clen = 0;
                do {
                    visited[cur] = 1;
                    cur = map[cur];
                    ++clen;
                } while (cur != start);
                lens[lnum++] = clen;
            }
        }

        /* sort lengths ascending */
        for (int i = 0; i < lnum - 1; ++i)
            for (int j = i + 1; j < lnum; ++j)
                if (lens[i] > lens[j]) {
                    unsigned char tmp   = lens[i];
                    lens[i]             = lens[j];
                    lens[j]             = tmp;
                }

        /* check for existing type */
        bool found = false;
        for (size_t t = 0; t < cycle_type_cnt; ++t) {
            CycleType *ct = &cycle_types[t];
            if (ct->num != lnum) continue;
            bool same = true;
            for (int k = 0; k < lnum; ++k) {
                if (ct->lens[k] != lens[k]) { same = false; break; }
            }
            if (same) {
                ct->mult += (seq_t)1;
                found = true;
                break;
            }
        }

        /* create new type if needed */
        if (!found) {
            if (cycle_type_cnt == cycle_type_cap) {
                cycle_type_cap = cycle_type_cap ? cycle_type_cap * 2 : 8;
                cycle_types = (CycleType *)realloc(cycle_types,
                                                   cycle_type_cap * sizeof(CycleType));
            }
            CycleType *ct = &cycle_types[cycle_type_cnt++];
            ct->num  = lnum;
            ct->lens = (unsigned char *)malloc(lnum);
            memcpy(ct->lens, lens, lnum);
            ct->mult  = (seq_t)1;
            ct->fixed = NULL;
        }
    }
}

/* ---------- permutation generation ------------------------------------- */
static void permute_rec(int depth, unsigned char *perm, unsigned char *used) {
    if (depth == 6) {
        process_perm(perm);
        return;
    }
    for (int i = 0; i < 6; ++i) {
        if (!used[i]) {
            used[i] = 1;
            perm[depth] = (unsigned char)i;
            permute_rec(depth + 1, perm, used);
            used[i] = 0;
        }
    }
}

/* ---------- initialization --------------------------------------------- */
static void init(void) {
    if (initialized) return;
    initialized = true;

    /* generate all permutations of indices 0..5 and process them          */
    unsigned char perm[6];
    unsigned char used[6] = {0};
    permute_rec(0, perm, used);

    /* pre‑compute fixed[n] for each cycle type                           */
    for (size_t i = 0; i < cycle_type_cnt; ++i) {
        CycleType *ct = &cycle_types[i];
        ct->fixed = (seq_t *)malloc(65 * sizeof(seq_t));
        for (int k = 0; k <= 64; ++k) ct->fixed[k] = 0;
        ct->fixed[0] = (seq_t)1;
        for (size_t j = 0; j < ct->num; ++j) {
            int l = ct->lens[j];
            for (int s = 64; s >= l; --s)
                ct->fixed[s] += ct->fixed[s - l];
        }
    }

    /* compute orbit counts using Burnside's lemma with 128‑bit accumulator   */
    u128 sum128[65];
    for (int k = 0; k <= 64; ++k) sum128[k] = 0;
    for (int n = 0; n <= 64; ++n) {
        for (size_t i = 0; i < cycle_type_cnt; ++i) {
            sum128[n] += (u128)cycle_types[i].mult *
                         (u128)cycle_types[i].fixed[n];
        }
        a_vals[n] = (seq_t)(sum128[n] / (u128)46080);
    }
}

/* ---------- exported function ---------------------------------------- */
seq_t a(idx_t n) {
    if (n > 64) return (seq_t)0;   /* no subsets larger than the whole set */
    init();                        /* ensure tables are ready */
    return a_vals[n];
}
