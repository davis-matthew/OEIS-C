/*
 * A202642  Number of (n+2)X5 binary arrays avoiding patterns 000 and 111 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A202642: Count of (n+2)×5 binary arrays avoiding 000 or 111
   in each row, column, and in all NW‑to‑SE diagonals.
   The implementation uses dynamic programming over the 16
   admissible 5‑bit rows.  For each ordered pair of rows
   (p,q) the code pre‑computes the set of rows r that can
   follow, i.e. for which the three rows (p,q,r) satisfy
   all vertical and diagonal restrictions.  Then it
   iteratively extends the matrix from height 2 to the
   required height H = n+2.  Complexity: O((n+2)·256)
   operations; memory usage is O(1).  The routine works
   for any seq_t / idx_t type. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------  Precomputation of good rows and transitions --------- */
static uint8_t  good_rows[16];   /* the 16 admissible 5‑bit patterns */
static size_t  good_row_cnt = 0; /* must be 16 */

/* transition mask: nextMask[p*16+q] has bit k set iff
   row k may follow the pair (p,q).  The mask is 16 bits. */
static uint16_t nextMask[256];
static bool     trans_precomp = false;

/* check that a 5‑bit row contains no horizontal run of
   three identical bits (000 or 111) */
static inline bool is_valid_row(uint8_t r)
{
    for (int i = 0; i < 3; ++i) {
        uint8_t block = (r >> i) & 7u;
        if (block == 0u || block == 7u)
            return false;
    }
    return true;
}

/* check that three rows (a,b,c) satisfy the vertical and
   NW‑to‑SE diagonal restrictions.  Horizontal restrictions
   are already enforced on all individual rows. */
static inline bool triple_ok(uint8_t a, uint8_t b, uint8_t c)
{
    /* column restriction: no column of three equal bits */
    for (int col = 0; col < 5; ++col) {
        uint8_t va = (a >> col) & 1u;
        uint8_t vb = (b >> col) & 1u;
        uint8_t vc = (c >> col) & 1u;
        if (va == vb && vb == vc)
            return false;
    }
    /* NW‑to‑SE diagonal restriction */
    for (int col = 0; col < 3; ++col) {
        uint8_t va = (a >> col) & 1u;
        uint8_t vb = (b >> (col + 1)) & 1u;
        uint8_t vc = (c >> (col + 2)) & 1u;
        if (va == vb && vb == vc)
            return false;
    }
    return true;
}

/* build the list of good rows and the transition table */
static void precompute(void)
{
    if (trans_precomp)
        return;

    /* collect all admissible 5‑bit rows (there are 16) */
    for (uint8_t r = 0; r < 32; ++r)
        if (is_valid_row(r))
            good_rows[good_row_cnt++] = r;

    /* build transition masks for each ordered pair of rows */
    for (uint16_t s = 0; s < 256; ++s) {
        uint8_t p = (s >> 4) & 15u;
        uint8_t q = s & 15u;
        uint8_t row_p = good_rows[p];
        uint8_t row_q = good_rows[q];
        uint16_t mask = 0;
        for (uint8_t k = 0; k < good_row_cnt; ++k) {
            uint8_t row_k = good_rows[k];
            if (triple_ok(row_p, row_q, row_k))
                mask |= (1u << k);
        }
        nextMask[s] = mask;
    }
    trans_precomp = true;
}

/* ---------  Main sequence function -------------------------------- */

seq_t a(idx_t n)
{
    /* The array has height H = n+2 rows.  For n < 1 the
       definition is not applicable; return 0. */
    if (n < 1)
        return (seq_t)0;

    idx_t H = n + 2;              /* number of rows */

    /* initialise transition tables if not already done */
    precompute();

    /* DP arrays: counts for each pair (previous, current)
       state.  Use static to keep the arrays persistent but
       re‑initialise them on every call. */
    static seq_t cur[256];
    static seq_t nxt[256];
    size_t i = 0;

    /* initialisation: after first two rows, each pair is
       admissible exactly once. */
    for (i = 0; i < 256; ++i)
        cur[i] = (seq_t)1;

    /* iterate over the remaining rows (row indices 2 .. H-1) */
    for (idx_t row = 2; row < H; ++row) {
        /* clear nxt */
        for (i = 0; i < 256; ++i)
            nxt[i] = (seq_t)0;

        /* for every current pair (p,q) propagate to next rows r */
        for (i = 0; i < 256; ++i) {
            seq_t val = cur[i];
            if (val == (seq_t)0)
                continue;
            uint16_t mask = nextMask[(uint16_t)i];
            /* iterate set bits of mask */
            uint16_t m = mask;
            while (m) {
                uint16_t lsb = m & (uint16_t)(- (int16_t) m);
                unsigned int k = __builtin_ctz((unsigned)m);
                uint8_t q = i & 15u;
                uint8_t new_state = (q << 4) | k;   /* (q,r) */
                nxt[new_state] += val;
                m ^= lsb;
            }
        }

        /* copy nxt into cur for the next iteration */
        for (i = 0; i < 256; ++i)
            cur[i] = nxt[i];
    }

    /* total number of arrays: sum over all final state counts */
    seq_t total = (seq_t)0;
    for (i = 0; i < 256; ++i)
        total += cur[i];

    return total;
}
