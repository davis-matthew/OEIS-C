/*
 * A207864  Number of n X 2 nonnegative integer arrays with new values 0
 *          upwards introduced in row major order and no element equal to any
 *          horizontal or vertical neighbor (colorings ignoring permutations
 *          of colors).
 * https://oeis.org/A207864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of stable partitions of the n‑ladder graph
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* ---------- helper: ensure we have space for at least 'need' elements ----------
   elem_size: size of each element.
   reallocates the block if necessary and updates *cap.  */
static void
ensure_cap(size_t *cap, void **arr, size_t need, size_t elem_size)
{
    if (need <= *cap)
        return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 1;
    if (new_cap < need)
        new_cap = need;
    *arr = (void *)realloc(*arr, new_cap * elem_size);
    *cap = new_cap;
}

/* ---------- global state for memoisation ---------- */
static seq_t *memo = NULL;        /* memo[i] holds a(i)      */
static size_t memo_cap   = 0;     /* allocated capacity      */
static idx_t  memo_last  = 0;     /* largest index computed  */

static seq_t *Bell    = NULL;   /* Bell numbers */
static size_t Bell_cap   = 0;
static idx_t  Bell_size  = 0;    /* number of entries stored (Bell[0..Bell_size-1]) */

static seq_t *deltaBell = NULL;   /* ΔBell[j] = Bell[j+2] – Bell[j+1] */
static size_t deltaBell_cap   = 0;
static idx_t  deltaBell_size = 0; /* number of entries stored   */

static seq_t *poly     = NULL;  /* current polynomial coefficients */
static seq_t *poly_next= NULL;  /* temporary buffer for next step */
static size_t poly_cap   = 0;
static idx_t  poly_len   = 0;    /* number of coefficients currently stored */

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* below offset is undefined */

    /* Expand memoisation up to n */
    for (idx_t cur = memo_last + 1; cur <= n; ++cur) {

        /* --- 1. Extend Bell numbers up to index 2*cur --- */
        idx_t needBell = (cur > 0) ? 2 * cur : 0;
        if ((size_t)(needBell + 1) > Bell_size) {          /* need indices 0..needBell inclusive */
            size_t new_cap = Bell_cap ? Bell_cap : 1;
            while (new_cap < (size_t)(needBell + 1))
                new_cap <<= 1;
            Bell = (seq_t *)realloc(Bell, new_cap * sizeof(seq_t));
            Bell_cap = new_cap;
            for (idx_t i = Bell_size; i <= needBell; ++i) {
                if (i == 0) {
                    Bell[0] = 1;
                } else if (i == 1) {
                    Bell[1] = 1;
                } else {
                    seq_t sum = 0;
                    seq_t binom = 1;   /* C(i-1,0) */
                    for (idx_t k = 0; k < i; ++k) {
                        sum += binom * Bell[k];
                        /* update binom = binom * (i-1-k) / (k+1) */
                        binom = binom * (i - 1 - k);
                        binom = binom / (k + 1);
                    }
                    Bell[i] = sum;
                }
            }
            Bell_size = needBell + 1;
        }

        /* --- 2. Extend deltaBell up to index 2*(cur-1) --- */
        idx_t needDelta = (cur > 0) ? 2 * (cur - 1) : 0;
        if ((size_t)(needDelta + 1) > deltaBell_size) {
            size_t new_cap = deltaBell_cap ? deltaBell_cap : 1;
            while (new_cap < (size_t)(needDelta + 1))
                new_cap <<= 1;
            deltaBell = (seq_t *)realloc(deltaBell, new_cap * sizeof(seq_t));
            deltaBell_cap = new_cap;
            for (idx_t j = deltaBell_size; j <= needDelta; ++j) {
                deltaBell[j] = Bell[j + 2] - Bell[j + 1];
            }
            deltaBell_size = needDelta + 1;
        }

        /* --- 3. Update polynomial coefficients --- */
        if (cur == 1) {
            /* P_0(x) = 1  --> coefficients: [1] */
            if (poly_cap < 1) {
                poly_cap = 1;
                poly = (seq_t *)realloc(poly, poly_cap * sizeof(seq_t));
                poly_next = (seq_t *)realloc(poly_next, poly_cap * sizeof(seq_t));
            }
            poly[0] = 1;
            poly_len = 1;
        } else {
            idx_t new_len = 2 * cur - 1;   /* length of coefficients for P_{cur-1} */
            if ((size_t)new_len > poly_cap) {
                size_t new_cap = poly_cap ? poly_cap : 1;
                while (new_cap < (size_t)new_len)
                    new_cap <<= 1;
                poly = (seq_t *)realloc(poly, new_cap * sizeof(seq_t));
                poly_next = (seq_t *)realloc(poly_next, new_cap * sizeof(seq_t));
                poly_cap = new_cap;
            }
            for (idx_t j = 0; j < new_len; ++j) {
                seq_t t0 = (j < poly_len) ? poly[j] : 0;                     /* poly[j]   */
                seq_t t1 = (j >= 1 && (j - 1) < poly_len) ? poly[j - 1] : 0; /* poly[j-1] */
                seq_t t2 = (j >= 2 && (j - 2) < poly_len) ? poly[j - 2] : 0; /* poly[j-2] */
                poly_next[j] = t2 - 3 * t1 + 3 * t0;                         /* new[j]    */
            }
            /* swap pointers for next step */
            seq_t *tmp = poly;
            poly = poly_next;
            poly_next = tmp;
            poly_len = new_len;
        }

        /* --- 4. Compute a(cur) from current coefficients --- */
        seq_t val = 0;
        for (idx_t j = 0; j < poly_len; ++j) {
            val += poly[j] * deltaBell[j];
        }

        /* --- 5. Store in memo table --- */
        if (memo_cap < (size_t)(cur + 1)) {
            size_t new_cap = memo_cap ? memo_cap : 1;
            while (new_cap < (size_t)(cur + 1))
                new_cap <<= 1;
            memo = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
            memo_cap = new_cap;
        }
        memo[cur] = val;
        memo_last = cur;
    }

    return memo[n];
}
