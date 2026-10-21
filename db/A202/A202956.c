/*
 * A202956  Number of arrays of n+2 integers in -2..2 with sum zero and
 *          adjacent elements differing in absolute value.
 * https://oeis.org/A202956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of arrays of length n+2 with elements in
   {-2,-1,0,1,2}, sum zero, and adjacent elements having different
   absolute values.  The value is computed by dynamic programming over
   length and sum, keeping a DP array for each absolute‐value type
   (0,1,2).  The code memoises results for all 1 ≤ n ≤ MAX_N.
   The arrays are bounded for a maximum length of MAX_L = MAX_N+2;
   MAX_N may be changed if a larger range is needed. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* ---------- Configuration ---------- */
#define MAX_N 10000                          /* maximum index supported   */
#define MAX_L (MAX_N + 2)                    /* maximum array length      */
#define MAX_SUM (2 * MAX_L)                  /* maximum possible absolute sum   */
#define OFF (MAX_SUM)                        /* offset to allow negative sums   */
#define ARRAY_SZ (2 * MAX_SUM + 1)           /* size of sum arrays                */

/* ---------- Absolute‑value types ---------- */
static const int abs_val[3] = {0, 1, 2};

/* ---------- DP storage (using pointers for swapping) ---------- */
static seq_t *cur[3];
static seq_t *nxt[3];
static bool initialized = false;

/* ---------- Cache for computed a(n) ---------- */
static seq_t cache[MAX_N + 1];
static bool cached[MAX_N + 1];

/* ---------- Current length of DP (initially 1 element) ---------- */
static size_t cur_len = 0;

/* ---------- Helper to zero an array of seq_t ---------- */
static void clear_arr(seq_t *p)
{
    for (size_t i = 0; i < ARRAY_SZ; ++i)
        p[i] = (seq_t)0;
}

/* ---------- Initialise DP arrays and cache ---------- */
static void init(void)
{
    if (initialized)
        return;
    initialized = true;

    /* allocate DP arrays */
    for (int t = 0; t < 3; ++t) {
        cur[t] = (seq_t *)malloc(sizeof(seq_t) * ARRAY_SZ);
        nxt[t] = (seq_t *)malloc(sizeof(seq_t) * ARRAY_SZ);
        clear_arr(cur[t]);
        clear_arr(nxt[t]);

        /* initialise length‑1 DP (single element) */
        if (t == 0)          /* value 0 */
            cur[t][OFF] = (seq_t)1;
        else if (t == 1) {   /* +1 or -1 */
            cur[t][OFF + 1] = (seq_t)1;
            cur[t][OFF - 1] = (seq_t)1;
        } else {             /* +2 or -2 */
            cur[t][OFF + 2] = (seq_t)1;
            cur[t][OFF - 2] = (seq_t)1;
        }
    }
    cur_len = 1;

    /* initialise cache */
    for (size_t i = 0; i <= MAX_N; ++i)
        cached[i] = false;
}

/* ---------- Extend DP to a desired length ---------- */
static void extend_to(size_t desired_len)
{
    while (cur_len < desired_len) {
        /* zero next arrays */
        for (int t = 0; t < 3; ++t)
            clear_arr(nxt[t]);

        /* transition from cur_len to cur_len+1 */
        for (int from = 0; from < 3; ++from) {
            const int a_from = abs_val[from];
            for (int idx = 0; idx < ARRAY_SZ; ++idx) {
                seq_t val = cur[from][idx];
                if (!val)
                    continue;
                for (int to = 0; to < 3; ++to) {
                    if (to == from)
                        continue;
                    int a_to = abs_val[to];
                    if (a_to == 0) {
                        /* value 0: only one sign */
                        nxt[to][idx] += val;
                    } else {
                        int idx1 = idx + a_to;
                        int idx2 = idx - a_to;
                        if ((size_t)idx1 < ARRAY_SZ)
                            nxt[to][idx1] += val;
                        if (idx2 >= 0)
                            nxt[to][idx2] += val;
                    }
                }
            }
        }

        /* swap cur and nxt pointers */
        for (int t = 0; t < 3; ++t) {
            seq_t *tmp = cur[t];
            cur[t] = nxt[t];
            nxt[t] = tmp;
        }
        ++cur_len;
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    if (n > MAX_N)
        return (seq_t)0;    /* out of supported range */

    init();

    size_t desired_len = (size_t)n + 2;
    if (desired_len > MAX_L)
        desired_len = MAX_L;

    if (cur_len < desired_len)
        extend_to(desired_len);

    if (cached[n])
        return cache[n];

    /* sum over all types for zero sum (offset index OFF) */
    seq_t result = (seq_t)0;
    for (int t = 0; t < 3; ++t)
        result += cur[t][OFF];

    cache[n] = result;
    cached[n] = true;
    return result;
}
