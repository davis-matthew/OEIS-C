/*
 * A207616  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A207617; see the Formula section.
 * https://oeis.org/A207616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   Triangular array defined by the recurrences u(1,x)=1, v(1,x)=1
 *            u(n,x)=u(n‑1,x)+v(n‑1,x) v(n,x)=x*u(n‑1,x)+v(n‑1,x)+1 a(n) is
 *            the flattened sequence of the u‑coefficients, i.e. a(n) = u(k,x)
 *            at the (k‑1)-th row's coefficients, in row‑major order
 * Range:     u8 n <= 27, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation uses dynamic arrays with a cache of the flattened
    coefficients.  The algorithm runs in O(n) time and uses O(n) memory.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* --- Parameters for memory allocation ------------------------------------- */
static const size_t INITIAL_CACHE_CAP = 1024;

/* Maximum number of coefficients in a row we expect to handle (≈ 2n).
   Set high enough for n up to about 40000; the contest limits are n ≤ 10000. */
static const size_t MAX_ROW_CAP = 20000;

/* --- Static state -------------------------------------------------------- */
static seq_t *cache   = NULL;   /* flattened coefficients (a(1), a(2), …)   */
static size_t cache_cap = 0;    /* capacity of cache                       */
static size_t cache_len = 0;    /* number of terms stored                  */

static seq_t *u_curr = NULL;   /* coefficients of u(n,x)   */
static seq_t *v_curr = NULL;   /* coefficients of v(n,x)   */
static seq_t *u_next = NULL;
static seq_t *v_next = NULL;
static size_t u_len = 0, v_len = 0;
static idx_t cur_row = 1;      /* current computed row (u_curr corresponds to this row) */

/* --- Helper: ensure cache has enough space -------------------------------- */
static void ensure_cache_cap(size_t needed)
{
    while (cache_cap < needed) {
        size_t new_cap = cache_cap == 0 ? INITIAL_CACHE_CAP : cache_cap * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* --- Initialization ------------------------------------------------------- */
static void init_static(void)
{
    if (cache != NULL) return;          /* already initialized */

    /* Allocate cache */
    cache_cap = INITIAL_CACHE_CAP;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();
    cache_len = 0;

    /* Allocate coefficient arrays */
    u_curr = (seq_t*)malloc(MAX_ROW_CAP * sizeof(seq_t));
    v_curr = (seq_t*)malloc(MAX_ROW_CAP * sizeof(seq_t));
    u_next = (seq_t*)malloc(MAX_ROW_CAP * sizeof(seq_t));
    v_next = (seq_t*)malloc(MAX_ROW_CAP * sizeof(seq_t));
    if (!u_curr || !v_curr || !u_next || !v_next) abort();

    /* Initial row n = 1 */
    u_curr[0] = (seq_t)1;
    v_curr[0] = (seq_t)1;
    u_len = 1;
    v_len = 1;
    cur_row = 1;

    /* Store first term */
    ensure_cache_cap(1);
    cache[0] = (seq_t)1;
    cache_len = 1;
}

/* --- Main sequence function ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* Undefined for n < 1 */

    init_static();

    /* Compute rows until we have at least n terms in the cache */
    while ((idx_t)cache_len < n) {
        idx_t next_row = cur_row + 1;
        size_t next_u_len = (next_row + 1) / 2;
        size_t next_v_len = next_row / 2 + 1;

        /* Compute u_next: u(n) = u(n-1) + v(n-1) */
        for (size_t i = 0; i < next_u_len; ++i) {
            seq_t val = (seq_t)0;
            if (i < u_len) val += u_curr[i];
            if (i < v_len) val += v_curr[i];
            u_next[i] = val;
        }

        /* Compute v_next: v(n) = x*u(n-1) + v(n-1) + 1 */
        for (size_t i = 0; i < next_v_len; ++i) {
            seq_t val = (seq_t)0;
            if (i == 0) {
                if (v_len > 0) val += v_curr[0];
                val += (seq_t)1;
            } else {
                if (i - 1 < u_len) val += u_curr[i - 1];
                if (i < v_len)   val += v_curr[i];
            }
            v_next[i] = val;
        }

        /* Append the new row's u-coefficients to the cache */
        ensure_cache_cap(cache_len + next_u_len);
        for (size_t i = 0; i < next_u_len; ++i) {
            cache[cache_len++] = u_next[i];
        }

        /* Swap current and next arrays for the next iteration */
        seq_t *tmp;
        tmp = u_curr; u_curr = u_next; u_next = tmp;
        tmp = v_curr; v_curr = v_next; v_next = tmp;

        u_len = next_u_len;
        v_len = next_v_len;
        cur_row = next_row;
    }

    return cache[n - 1];
}
