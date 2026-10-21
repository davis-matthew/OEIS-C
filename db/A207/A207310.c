/*
 * A207310  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 7×n 0/1 arrays avoiding 0 0 0 and 1 0 1
 *            horizontally and 0 0 1 and 1 1 0 vertically
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define COLS          7                 /* height of the array */
#define MAXCOL        (1U << COLS)       /* 128 possible columns 0 … 127 */

typedef struct { uint64_t lo; uint64_t hi; } mask_t;

/* Static state: computed once and reused for all calls. */
static bool      enabled      = false;
static bool      allowed[MAXCOL];
static mask_t    adjacency[MAXCOL][MAXCOL];

static void init(void)
{
    /* determine allowed columns respecting vertical patterns */
    for (unsigned c = 0; c < MAXCOL; ++c) {
        bool ok = true;
        for (unsigned i = 0; i + 2 < COLS; ++i) {
            unsigned b0 = (c >> i)     & 1;
            unsigned b1 = (c >> (i+1)) & 1;
            unsigned b2 = (c >> (i+2)) & 1;
            if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                (b0 == 1 && b1 == 1 && b2 == 0)) {
                ok = false; break;
            }
        }
        allowed[c] = ok;
    }

    /* build adjacency masks for horizontal constraints */
    for (unsigned p = 0; p < MAXCOL; ++p) {
        if (!allowed[p]) continue;
        for (unsigned q = 0; q < MAXCOL; ++q) {
            if (!allowed[q]) continue;
            mask_t m = {0,0};
            for (unsigned r = 0; r < MAXCOL; ++r) {
                if (!allowed[r]) continue;
                bool ok = true;
                for (unsigned i = 0; i < COLS; ++i) {
                    unsigned bp = (p >> i) & 1;
                    unsigned bq = (q >> i) & 1;
                    unsigned br = (r >> i) & 1;
                    if ((bp == 0 && bq == 0 && br == 0) ||
                        (bp == 1 && bq == 0 && br == 1)) {
                        ok = false; break;
                    }
                }
                if (ok) {
                    if (r < 64)
                        m.lo |= (1ULL << r);
                    else
                        m.hi |= (1ULL << (r - 64));
                }
            }
            adjacency[p][q] = m;
        }
    }
    enabled = true;
}

seq_t a(idx_t n)
{
    /* Ensure initialization only once. */
    if (!enabled) init();

    /* count of allowed columns: a(1) */
    static unsigned num_allowed = 0;
    if (num_allowed == 0) {
        for (unsigned i = 0; i < MAXCOL; ++i)
            if (allowed[i]) ++num_allowed;
    }

    if (n == 1) return (seq_t)num_allowed;

    /* DP arrays: dp[p][q] = number of ways ending with columns p,q */
    static seq_t dp[MAXCOL][MAXCOL];
    static seq_t ndp[MAXCOL][MAXCOL];

    /* initialise for two columns */
    for (unsigned p = 0; p < MAXCOL; ++p)
        for (unsigned q = 0; q < MAXCOL; ++q)
            dp[p][q] = (allowed[p] && allowed[q]) ? (seq_t)1 : (seq_t)0;

    /* iterate over remaining columns */
    for (unsigned col = 3; col <= (unsigned)n; ++col) {
        /* clear ndp */
        for (unsigned i = 0; i < MAXCOL; ++i)
            for (unsigned j = 0; j < MAXCOL; ++j)
                ndp[i][j] = (seq_t)0;

        /* transition */
        for (unsigned p = 0; p < MAXCOL; ++p) {
            if (!allowed[p]) continue;
            for (unsigned q = 0; q < MAXCOL; ++q) {
                if (!allowed[q]) continue;
                seq_t cur = dp[p][q];
                if (cur == 0) continue;
                mask_t m = adjacency[p][q];
                /* low 64 bits */
                uint64_t bits = m.lo;
                while (bits) {
                    unsigned r = __builtin_ctzll(bits);
                    ndp[q][r] += cur;
                    bits &= bits - 1;
                }
                /* high 64 bits */
                bits = m.hi;
                while (bits) {
                    unsigned r = 64 + __builtin_ctzll(bits);
                    ndp[q][r] += cur;
                    bits &= bits - 1;
                }
            }
        }

        /* copy ndp back to dp for next iteration */
        for (unsigned i = 0; i < MAXCOL; ++i)
            for (unsigned j = 0; j < MAXCOL; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* sum all states to obtain the total count */
    seq_t total = (seq_t)0;
    for (unsigned p = 0; p < MAXCOL; ++p)
        for (unsigned q = 0; q < MAXCOL; ++q)
            total += dp[p][q];

    return total;
}
