/*
 * A204208  Number of length n+1 nonnegative integer arrays starting and
 *          ending with 0 with adjacent elements differing by no more than 3.
 * https://oeis.org/A204208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A204208 – Number of length n + 1 non‑negative integer arrays
 *  starting and ending with 0 with adjacent elements differing by
 *  at most 3.
 *
 *  Computation method:
 *  Let dp[i][h] be the number of walks from 0 to height h using
 *  i steps where each step is in {‑3,…,3} and all intermediate
 *  heights are non‑negative.  Then
 *
 *      dp[0][0] = 1,  dp[0][h>0] = 0
 *
 *  and for i ≥ 1
 *
 *      dp[i][h] = Σ_{k=‑3}^{3} dp[i‑1][h‑k]   (with h‑k ≥ 0)
 *
 *  The desired value is a(n) = dp[n][0].
 *
 *  The implementation keeps a single row of dp in two buffers
 *  (cur and next), updating each new row from the previous one
 *  in O(3 · i) time.  All values are stored in the type seq_t
 *  (which can be an arbitrary‑precision integer class).  The
 *  function is memoised so that repeated calls to a(n) do not
 *  recompute earlier terms.  The code compiles both as C11 and
 *  as C++17 – the allocation helpers below use C++ new when
 *  available and POSIX malloc otherwise.
 */

#ifndef __A204208_C__
#define __A204208_C__

#ifndef NULL
#  include <stddef.h>
#endif
#ifndef uint64_t
#  include <stdint.h>
#endif

/* ----------------------------------------------------------------
 *  Allocation helpers – portable between C and C++.
 *  --------------------------------------------------------------*/
static seq_t *alloc_seq_array(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n]();
#else
    return (seq_t*)calloc(n, sizeof(seq_t));
#endif
}

static seq_t *realloc_seq_array(seq_t *p, size_t oldn, size_t newn)
{
#ifdef __cplusplus
    if (!p) return new seq_t[newn]();
    seq_t *q = new seq_t[newn]();
    for (size_t i = 0; i < oldn; ++i) q[i] = p[i];
    delete[] p;
    return q;
#else
    return (seq_t*)realloc(p, newn * sizeof(seq_t));
#endif
}

static void free_seq_array(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* ----------------------------------------------------------------
 *  Memoisation and DP buffers.
 *  --------------------------------------------------------------*/
static seq_t *memo = NULL;     /* stores a(1)..a(mem_len) */
static size_t mem_len = 0;     /* number of terms stored */
static size_t mem_cap = 0;     /* allocation size of memo */

static seq_t *cur  = NULL;     /* DP for step i   */
static seq_t *next = NULL;     /* DP for step i+1 */
static size_t dp_cap = 0;      /* allocation size of cur/next */
static idx_t last_n = 0;       /* steps already processed */

/* ----------------------------------------------------------------
 *  Ensure memo array can hold at least `need` terms.
 *  --------------------------------------------------------------*/
static void ensure_memo_size(idx_t need)
{
    if ((size_t)need <= mem_cap) return;
    size_t new_cap = mem_cap ? mem_cap * 2 : 1;
    while (new_cap < (size_t)need) new_cap <<= 1;
    seq_t *new_mem = realloc_seq_array(memo, mem_cap, new_cap);
    memo = new_mem;
    mem_cap = new_cap;
}

/* ----------------------------------------------------------------
 *  Ensure DP buffers can hold at least `need` heights (indices 0..need‑1).
 *  --------------------------------------------------------------*/
static void ensure_dp_size(size_t need)
{
    if (need <= dp_cap) return;
    size_t new_cap = dp_cap ? dp_cap << 1 : 4;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_cur  = realloc_seq_array(cur, dp_cap, new_cap);
    seq_t *new_next = realloc_seq_array(next, dp_cap, new_cap);
    cur  = new_cur;
    next = new_next;
    dp_cap = new_cap;
}

/* ----------------------------------------------------------------
 *  Main function – returns a(n) for n ≥ 1.
 *  --------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1 – safety */

    ensure_memo_size(n);

    /* Initialise DP on first call */
    if (cur == NULL) {
        dp_cap = 1;
        cur  = alloc_seq_array(dp_cap);
        next = alloc_seq_array(dp_cap);
        cur[0] = (seq_t)1;          /* dp[0][0] = 1 */
        last_n = 0;
        mem_len = 0;
    }

    /* Grow memo if we need more slots than already allocated */
    if ((size_t)n > mem_cap)
        ensure_memo_size(n);

    /* Compute steps up to n */
    while (last_n < n) {
        idx_t step         = last_n + 1;               /* next number of steps */
        idx_t maxH_cur     = 3 * last_n;               /* max height reachable in cur */
        idx_t maxH_next    = 3 * step;                 /* max height after new step */

        ensure_dp_size((size_t)maxH_next + 1);

        /* Compute next[0] – sum of cur[0]..cur[min(3,maxH_cur)] */
        seq_t sum = 0;
        idx_t up = maxH_cur < 3 ? maxH_cur : 3;
        for (idx_t h = 0; h <= up; ++h) sum += cur[h];
        next[0] = sum;

        /* Sliding window for heights 1 .. maxH_next */
        for (idx_t h = 1; h <= maxH_next; ++h) {
            if (h >= 4 && h - 4 <= maxH_cur) sum -= cur[h - 4];
            if (h + 3 <= maxH_cur)           sum += cur[h + 3];
            next[h] = sum;
        }

        /* Swap buffers for next iteration */
        seq_t *tmp = cur;
        cur  = next;
        next = tmp;

        /* Store result for this step */
        mem_len = step;                 /* at least this many terms stored */
        if (step <= mem_cap) memo[step - 1] = cur[0];

        last_n = step;
    }

    return memo[n - 1];  /* a(n) is stored at index n-1 */
}

#endif /* __A204208_C__ */
