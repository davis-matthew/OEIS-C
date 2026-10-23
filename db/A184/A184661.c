/*
 * A184661  Number of (n+2)X4 0..3 arrays with each 3X3 subblock singular.
 * https://oeis.org/A184661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 8, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A184661
 * Number of (n+2)×4 arrays over {0,1,2,3} whose every 3×3 subblock
 * is singular.  Computed via dynamic programming over pairs of rows.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer types provided by the build environment */
typedef uint64_t idx_t;

typedef struct {
    unsigned short *data;    /* list of next rows for a given pair */
    size_t          len;     /* number of entries in data */
} Transition;

static int det3(const uint8_t *ra, const uint8_t *rb, const uint8_t *rc, int start)
{
    int a0 = ra[start];
    int a1 = ra[start + 1];
    int a2 = ra[start + 2];
    int b0 = rb[start];
    int b1 = rb[start + 1];
    int b2 = rb[start + 2];
    int c0 = rc[start];
    int c1 = rc[start + 1];
    int c2 = rc[start + 2];
    return a0 * b1 * c2 + b0 * c1 * a2 + c0 * a1 * b2
         - c0 * b1 * a2 - a0 * c1 * b2 - b0 * a1 * c2;
}

seq_t a(idx_t n)
{
    /* One-time initialization of transition data and DP buffers.  */
    static int initialized = 0;
    static seq_t *cur = NULL;
    static seq_t *next = NULL;
    static Transition *trans = NULL;
    static seq_t *a_vals = NULL;
    static size_t a_capacity = 0;
    static size_t a_count = 0;                 /* number of terms already computed */

    if (!initialized)
    {
        const size_t ROWS = 256;               /* 4^4 possible rows (00..33) */
        const size_t S = ROWS * ROWS;          /* all ordered pairs of rows */

        /* Generate all row patterns.  */
        uint8_t rows[ROWS][4];
        for (size_t r = 0; r < ROWS; ++r)
            for (int c = 0; c < 4; ++c)
                rows[r][c] = (uint8_t)((r >> (2 * c)) & 3);

        /* First pass: count how many next rows are valid for each pair.  */
        size_t *cnt = (size_t *)malloc(S * sizeof(size_t));
        if (!cnt) return 0;
        for (size_t i = 0; i < S; ++i) cnt[i] = 0;
        for (size_t a = 0; a < ROWS; ++a)
            for (size_t b = 0; b < ROWS; ++b)
            {
                const uint8_t *ra = rows[a];
                const uint8_t *rb = rows[b];
                for (size_t c = 0; c < ROWS; ++c)
                {
                    const uint8_t *rc = rows[c];
                    if (det3(ra, rb, rc, 0) != 0) continue;
                    if (det3(ra, rb, rc, 1) != 0) continue;
                    ++cnt[(a << 8) | b];
                }
            }

        /* Allocate transition arrays.  */
        trans = (Transition *)malloc(S * sizeof(Transition));
        if (!trans) return 0;
        for (size_t i = 0; i < S; ++i)
        {
            trans[i].data = NULL;
            trans[i].len  = cnt[i];
            if (cnt[i] > 0)
                trans[i].data = (unsigned short *)malloc(cnt[i] * sizeof(unsigned short));
        }

        /* Second pass: fill transition lists.  */
        size_t *pos = (size_t *)calloc(S, sizeof(size_t));
        if (!pos) return 0;
        for (size_t a = 0; a < ROWS; ++a)
            for (size_t b = 0; b < ROWS; ++b)
            {
                const uint8_t *ra = rows[a];
                const uint8_t *rb = rows[b];
                size_t idx = (a << 8) | b;
                for (size_t c = 0; c < ROWS; ++c)
                {
                    const uint8_t *rc = rows[c];
                    if (det3(ra, rb, rc, 0) != 0) continue;
                    if (det3(ra, rb, rc, 1) != 0) continue;
                    trans[idx].data[pos[idx]++] = (unsigned short)c;
                }
            }
        free(pos);
        free(cnt);

        /* Allocate DP arrays for pairs.  */
        cur  = (seq_t *)malloc(S * sizeof(seq_t));
        next = (seq_t *)malloc(S * sizeof(seq_t));
        if (!cur || !next) return 0;

        /* Initial DP: all pairs are allowed before any 3‑row check.  */
        for (size_t i = 0; i < S; ++i) cur[i] = (seq_t)1;

        initialized = 1;
    }

    /* Compute terms up to request n.  */
    while (a_count < (size_t)n)
    {
        /* Reset next array.  */
        const size_t S = 256 * 256;
        for (size_t i = 0; i < S; ++i) next[i] = (seq_t)0;

        /* Transition step: for each current pair add contributions for all valid next rows.  */
        for (size_t i = 0; i < S; ++i)
        {
            if (cur[i] == (seq_t)0) continue;
            size_t b = i & 255;
            const Transition *t = &trans[i];
            for (size_t j = 0; j < t->len; ++j)
            {
                size_t dest = (b << 8) | t->data[j];
                next[dest] += cur[i];
            }
        }

        /* Sum all sequences of the current length (height = a_count+3).  */
        seq_t total = (seq_t)0;
        for (size_t i = 0; i < S; ++i) total += next[i];

        /* Store the computed term.  */
        if (a_count == a_capacity)
        {
            size_t new_cap = (a_capacity == 0) ? 16 : a_capacity << 1;
            seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
            if (!tmp) return 0;
            a_vals = tmp;
            a_capacity = new_cap;
        }
        a_vals[a_count++] = total;

        /* Prepare for next iteration.  */
        seq_t *tmp_ptr = cur;
        cur = next;
        next = tmp_ptr;
    }

    return a_vals[n - 1];
}
