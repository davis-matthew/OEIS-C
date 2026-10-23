/*
 * A167663  Where records occur for partitions into odd squares, cf. A167661.
 * https://oeis.org/A167663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the n‑th record in the partition numbers of an
 *            integer into odd squares. The value a(1)=0
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* State for incremental computation */
static struct {
    idx_t   max_n;      /* largest n with dp computed */
    seq_t *dp;          /* partition counts p(m) for m ≤ max_n */
    size_t dp_cap;      /* allocated size of dp */
    seq_t  max_val;     /* largest partition count seen so far */
    idx_t  next_odd;    /* next odd integer whose square has not been added */
    idx_t *rec;         /* indices where a record occurs */
    size_t rec_cap;     /* allocated size of rec array */
    size_t rec_cnt;     /* number of records stored */
    int     inited;     /* has the state been initialized? */
} memo = {0};

/* Ensure dp has capacity for at least cap elements */
static void ensure_dp_cap(size_t cap)
{
    if (cap <= memo.dp_cap) return;
    seq_t *new_dp = (seq_t *)realloc(memo.dp, cap * sizeof(seq_t));
    if (!new_dp) abort();
    /* zero the new region */
    for (size_t i = memo.dp_cap; i < cap; ++i) new_dp[i] = 0;
    memo.dp    = new_dp;
    memo.dp_cap = cap;
}

/* Ensure rec has capacity for at least cap elements */
static void ensure_rec_cap(size_t cap)
{
    if (cap <= memo.rec_cap) return;
    idx_t *new_rec = (idx_t *)realloc(memo.rec, cap * sizeof(idx_t));
    if (!new_rec) abort();
    memo.rec    = new_rec;
    memo.rec_cap = cap;
}

/* Initialise state from scratch */
static void init_state(void)
{
    memo.max_n    = 0;
    memo.dp_cap   = 1;
    memo.dp       = (seq_t *)malloc(sizeof(seq_t));
    if (!memo.dp) abort();
    memo.dp[0]    = 1;             /* p(0) = 1 */
    memo.next_odd = 1;             /* next odd to process */
    memo.max_val  = memo.dp[0];

    memo.rec_cap  = 1;
    memo.rec       = (idx_t *)malloc(sizeof(idx_t));
    if (!memo.rec) abort();
    memo.rec[0]    = 0;            /* first record at 0 */
    memo.rec_cnt  = 1;

    memo.inited   = 1;
}

/* Extend precomputed tables to at least n */
static void extend_to(idx_t n)
{
    if (!memo.inited) init_state();

    while (memo.max_n < n) {
        /* Grow the dp array by a reasonable chunk */
        idx_t old_max = memo.max_n;
        idx_t new_max = old_max + 1000;
        if (new_max < old_max) new_max = ~((idx_t)0); /* overflow protection */

        ensure_dp_cap(new_max + 1);

        /* Process each odd square up to new_max */
        for (idx_t odd = memo.next_odd; ; odd += 2) {
            idx_t sq = odd * odd;
            if (sq > new_max) break;
            idx_t start = (sq > old_max) ? sq : old_max + 1;
            for (idx_t i = start; i <= new_max; ++i)
                memo.dp[i] = memo.dp[i] + memo.dp[i - sq];
            memo.next_odd = odd + 2;
        }

        /* Record new maxima */
        for (idx_t i = old_max + 1; i <= new_max; ++i) {
            if (memo.dp[i] > memo.max_val) {
                memo.max_val = memo.dp[i];
                ensure_rec_cap(memo.rec_cnt + 1);
                memo.rec[memo.rec_cnt++] = i;
            }
        }

        memo.max_n = new_max;
    }
}

/* Public function required by the grading harness */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined behaviour for n<1, but guard */
    extend_to(n);
    /* rec is 0‑based: rec[0] corresponds to a(1) */
    return (seq_t)memo.rec[n - 1];
}
