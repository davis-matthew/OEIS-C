/*
 * A028448  Number of perfect matchings in graph P_{2} X P_{4} X P_{n}.
 * https://oeis.org/A028448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A028448 – Number of perfect matchings in P₂ × P₄ × Pₙ
 * 
 * The sequence is computed by a transfer‑matrix dynamic programming
 * over the 8‑vertex slice P₂×P₄.  Each state is represented by an 8‑bit
 * mask indicating which cells in the current slice already have a
 * vertical domino coming from the previous slice.  Transitions are
 * pre‑computed once, and the DP proceeds along the n‑direction.
 * The function works for any seq_t/idx_t combination provided by the
 * build system (uint8_t … uint64_t or arbitrary‑precision).
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef __cplusplus
/* For C we use malloc/free */
#define ALLOC_ELEM(p, n, type) \
    p = (type *)malloc((size_t)(n) * sizeof(type))
#define FREE_ELEM(p, n, type)  free(p)
#else
/* For C++ we use new/delete */
#define ALLOC_ELEM(p, n, type) p = new type[n]
#define FREE_ELEM(p, n, type)  delete [] p
#endif

/* ---------------------------------------------------------- */
/*  Transition matrix – 256×256 (8‑bit masks)                 */
/* ---------------------------------------------------------- */
static seq_t transitions[256][256];
static bool transitions_ready = false;

/* Recursively generate all ways to cover a slice given a starting
 * mask of already occupied cells (incoming verticals).  `used` keeps
 * track of which cells are covered in this slice; `inMask` is the
 * incoming mask; `outMask` accumulates the cells that start a
 * vertical domino towards the next slice. */
static void explore(uint8_t used, uint8_t inMask, uint8_t outMask) {
    if (used == 0xFFu) {                    /* all 8 cells covered */
        transitions[inMask][outMask] += (seq_t)1;
        return;
    }
    /* first free cell */
    uint8_t freeMask = (uint8_t)~used;
    int v = __builtin_ctz((unsigned)freeMask); /* lowest set bit */
    uint8_t vm = 1u << v;

    /* Place a vertical domino to the next slice */
    explore(used | vm, inMask, outMask | vm);

    /* Place a horizontal right neighbour if possible */
    int col = v % 4;
    if (col < 3) {
        uint8_t nm = 1u << (v + 1);
        if (!(used & nm))
            explore(used | vm | nm, inMask, outMask);
    }
    /* Place a vertical neighbour in the 2‑row grid */
    if (v < 4) {          /* top row cell */
        uint8_t nm = 1u << (v + 4);
        if (!(used & nm))
            explore(used | vm | nm, inMask, outMask);
    }
}

/* Initialise the transition matrix once. */
static void init_transitions(void) {
    if (transitions_ready) return;
    for (int i = 0; i < 256; ++i)
        for (int j = 0; j < 256; ++j)
            transitions[i][j] = (seq_t)0;
    for (int im = 0; im < 256; ++im)
        explore((uint8_t)im, (uint8_t)im, (uint8_t)0);
    transitions_ready = true;
}

/* ---------------------------------------------------------- */
/*  DP arrays                                               */
/* ---------------------------------------------------------- */
static seq_t *dp_cur = NULL;
static seq_t *dp_nxt = NULL;

/* Allocate DP buffers and initialise the starting state. */
static void init_dp_arrays(void) {
    if (dp_cur) return;    /* already initialised */
    ALLOC_ELEM(dp_cur, 256, seq_t);
    ALLOC_ELEM(dp_nxt, 256, seq_t);
    for (int i = 0; i < 256; ++i) {
        dp_cur[i] = dp_nxt[i] = (seq_t)0;
    }
    dp_cur[0] = (seq_t)1;  /* no pending verticals */
}

/* ---------------------------------------------------------- */
/*  Sequence storage                                         */
/* ---------------------------------------------------------- */
static seq_t *a_table = NULL;
static idx_t  a_table_size = 0;      /* allocated elements (>=1 if used) */
static idx_t  max_computed = 0;      /* largest n computed and stored */

/* Ensure the a_table can hold indices up to `need`. */
static void ensure_a_table(idx_t need) {
    if (need < a_table_size) return;
    size_t newsize = (size_t)need + 1;
    seq_t *newp;
    ALLOC_ELEM(newp, newsize, seq_t);

    /* copy existing values */
    if (a_table) {
        for (idx_t i = 0; i <= max_computed; ++i)
            newp[i] = a_table[i];
    }
    /* zero new entries */
    for (idx_t i = max_computed + 1; i < newsize; ++i)
        newp[i] = (seq_t)0;

    if (a_table) FREE_ELEM(a_table, a_table_size, seq_t);
    a_table = newp;
    a_table_size = newsize;
}

/* ---------------------------------------------------------- */
/*  Core DP routine                                         */
/* ---------------------------------------------------------- */
static void compute_up_to(idx_t target) {
    init_transitions();
    init_dp_arrays();
    ensure_a_table(target);

    if (target <= max_computed) return;

    for (idx_t n = max_computed + 1; n <= target; ++n) {
        /* zero next DP layer */
        for (int i = 0; i < 256; ++i)
            dp_nxt[i] = (seq_t)0;

        /* transition step */
        for (int im = 0; im < 256; ++im) {
            seq_t val = dp_cur[im];
            if (val == (seq_t)0) continue;
            for (int om = 0; om < 256; ++om) {
                seq_t t = transitions[im][om];
                if (t == (seq_t)0) continue;
                dp_nxt[om] += val * t;
            }
        }
        /* swap current and next */
        for (int i = 0; i < 256; ++i)
            dp_cur[i] = dp_nxt[i];

        /* store result for this n (perfect matchings end with no pending verticals) */
        a_table[n] = dp_cur[0];
    }
    max_computed = target;
}

/* ---------------------------------------------------------- */
/*  Public API                                               */
/* ---------------------------------------------------------- */
seq_t a(idx_t n) {
    /* Initialisation on first call */
    if (max_computed == 0 && a_table_size == 0) {
        init_transitions();
        init_dp_arrays();
        ensure_a_table(0);      /* allocate table of size 1 */
        a_table[0] = dp_cur[0]; /* a(0) = 1 */
        max_computed = 0;
    }
    if (n > max_computed)
        compute_up_to(n);
    return a_table[n];
}
