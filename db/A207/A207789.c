/*
 * A207789  Number of 7 X n 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 0 and 1 0 1 vertically.
 * https://oeis.org/A207789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 7×n 0..1 arrays avoiding * horizontal patterns
 *            0 0 0 and 0 1 1, and vertical patterns 0 0 0 and 1 0 1. *
 *            Computed by a linear‑time DP on column‑pairs. * a(1) = 28, a(2)
 *            = 784, … (see OEIS A207789)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 15, u64 n <= 41, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t Vsize = 0;                    /* number of valid 7‑bit columns   */
static unsigned char *valid = 0;           /* list of valid column patterns   */
static size_t S = 0;                       /* number of pair‑state (p,q) states */
static size_t *succ_offset = 0;            /* cumulative edge counts per state */
static uint16_t *succ_list = 0;            /* flat list of destination states */
static seq_t *cur = 0, *nxt = 0;           /* DP buffers                     */
static int initialized = 0;

/* ------------------------------------------------------------------ */
static void init(void) {
    if (initialized) return;

    /* 1) Build list of valid column patterns (vertical constraints) */
    unsigned char temp[128];
    size_t cnt = 0;
    for (idx_t mask = 0; mask < (idx_t)128; ++mask) {
        bool ok = true;
        for (idx_t i = 0; i + 2 < (idx_t)7; ++i) {
            int a = (mask >> i) & 1;
            int b = (mask >> (i + 1)) & 1;
            int c = (mask >> (i + 2)) & 1;
            if ((a == 0 && b == 0 && c == 0) || (a == 1 && b == 0 && c == 1)) {
                ok = false;
                break;
            }
        }
        if (ok) temp[cnt++] = (unsigned char)mask;
    }
    Vsize = cnt;
    valid = (unsigned char*)malloc(Vsize * sizeof(unsigned char));
    for (size_t i = 0; i < Vsize; ++i) valid[i] = temp[i];

    /* 2) Build transition graph for pairs of columns */
    S = Vsize * Vsize;
    succ_offset = (size_t*)malloc((S + 1) * sizeof(size_t));
    succ_offset[0] = 0;

    /* first pass: count successors per state */
    for (size_t s = 0; s < S; ++s) {
        size_t p = s / Vsize;
        size_t q = s % Vsize;
        unsigned char cp = valid[p];
        unsigned char cq = valid[q];
        size_t scnt = 0;
        unsigned char nm = (unsigned char)((~cp) & 0x7F); /* only 7 relevant bits */
        for (size_t r = 0; r < Vsize; ++r) {
            unsigned char cr = valid[r];
            if ( ((nm & (cr ^ cq)) == nm) ) ++scnt;
        }
        succ_offset[s + 1] = succ_offset[s] + scnt;
    }
    size_t total_edges = succ_offset[S];

    succ_list = (uint16_t*)malloc(total_edges * sizeof(uint16_t));

    /* second pass: fill successor list */
    for (size_t s = 0; s < S; ++s) {
        size_t p = s / Vsize;
        size_t q = s % Vsize;
        unsigned char cp = valid[p];
        unsigned char cq = valid[q];
        unsigned char nm = (unsigned char)((~cp) & 0x7F);
        size_t off = succ_offset[s];
        for (size_t r = 0; r < Vsize; ++r) {
            unsigned char cr = valid[r];
            if ( ((nm & (cr ^ cq)) == nm) ) {
                size_t t = q * Vsize + r;
                succ_list[off++] = (uint16_t)t;
            }
        }
    }

    /* 3) Allocate DP arrays */
    cur = (seq_t*)malloc(S * sizeof(seq_t));
    nxt = (seq_t*)malloc(S * sizeof(seq_t));

    initialized = 1;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    init();

    if (n == 0) return 0;                 /* offset is 1, ignore 0th call */
    if (n == 1) return (seq_t)Vsize;      /* a(1) = 28 */
    if (n == 2) return (seq_t)Vsize * (seq_t)Vsize; /* a(2) = 784 */

    /* initialize DP for n = 2 (all states have one array of length 2) */
    for (size_t s = 0; s < S; ++s) cur[s] = (seq_t)1;

    for (idx_t step = 3; step <= n; ++step) {
        /* clear nxt */
        for (size_t s = 0; s < S; ++s) nxt[s] = (seq_t)0;

        /* transition from cur to nxt */
        for (size_t s = 0; s < S; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            size_t off_start = succ_offset[s];
            size_t off_end   = succ_offset[s + 1];
            for (size_t idx = off_start; idx < off_end; ++idx) {
                size_t t = succ_list[idx];
                nxt[t] += val;
            }
        }

        /* swap buffers */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum all ways for length n */
    seq_t total = 0;
    for (size_t s = 0; s < S; ++s) total += cur[s];
    return total;
}
