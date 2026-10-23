/*
 * A165524  Number of permutations of length n which avoid the patterns 4321
 *          and 1324.
 * https://oeis.org/A165524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 30, u64 n <= 237,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165524: Number of permutations of length n avoiding patterns
   4321 and 1324.
   The sequence satisfies the linear recurrence

        a(n) = 12*a(n-1) - 66*a(n-2) + 220*a(n-3) - 495*a(n-4)
              + 792*a(n-5) - 924*a(n-6) + 792*a(n-7) - 495*a(n-8)
              + 220*a(n-9) - 66*a(n-10) + 12*a(n-11) - a(n-12)  for n > 12

   with initial values
        a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=22, a(5)=86, a(6)=332,
        a(7)=1217, a(8)=4140, a(9)=12934, a(10)=37088, a(11)=98115,
        a(12)=241269. */
   
#include <stdlib.h>

/* Memoisation cache for the recurrence.  */
static seq_t *memo = NULL;
static size_t memo_cap = 0;          /* current capacity of memo      */
static idx_t  computed_up_to = 0;    /* highest index that has a cached value */

/* The first 13 terms of the sequence.  */
static const seq_t init[13] = {
    (seq_t)1,      /* a(0) */
    (seq_t)1,      /* a(1) */
    (seq_t)2,      /* a(2) */
    (seq_t)6,      /* a(3) */
    (seq_t)22,     /* a(4) */
    (seq_t)86,     /* a(5) */
    (seq_t)332,    /* a(6) */
    (seq_t)1217,   /* a(7) */
    (seq_t)4140,   /* a(8) */
    (seq_t)12934,  /* a(9) */
    (seq_t)37088,  /* a(10) */
    (seq_t)98115,  /* a(11) */
    (seq_t)241269  /* a(12) */
};

/* Ensure the memo array contains values up to index n.  */
static void
ensure_memo(idx_t n)
{
    /* The cache must have space for indices 0..n.  */
    size_t required_cap = (size_t)n + 1;
    if (required_cap < 13) required_cap = 13;

    if (memo == NULL) {
        /* First allocation: copy the initial 13 values.  */
        memo = (seq_t *)malloc(required_cap * sizeof(seq_t));
        if (!memo) return;                 /* allocation failed; caller will use 0 */
        for (idx_t i = 0; i <= 12; ++i) memo[i] = init[i];
        memo_cap = required_cap;
        computed_up_to = 12;
    }
    else if (memo_cap < required_cap) {
        seq_t *tmp = (seq_t *)realloc(memo, required_cap * sizeof(seq_t));
        if (!tmp) return;
        memo = tmp;
        memo_cap = required_cap;
    }

    if (computed_up_to < 12) {
        /* In case the cache was allocated with 13 capacity but never filled. */
        for (idx_t i = 0; i <= 12; ++i) memo[i] = init[i];
        computed_up_to = 12;
    }

    /* Fill any missing terms up to n using the recurrence.  */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t pos =
            (seq_t)12  * memo[(size_t)(i-1)] +
            (seq_t)220 * memo[(size_t)(i-3)] +
            (seq_t)792 * memo[(size_t)(i-5)] +
            (seq_t)792 * memo[(size_t)(i-7)] +
            (seq_t)220 * memo[(size_t)(i-9)] +
            (seq_t)12  * memo[(size_t)(i-11)];

        seq_t neg =
            (seq_t)66  * memo[(size_t)(i-2)] +
            (seq_t)495 * memo[(size_t)(i-4)] +
            (seq_t)924 * memo[(size_t)(i-6)] +
            (seq_t)495 * memo[(size_t)(i-8)] +
            (seq_t)66  * memo[(size_t)(i-10)] +
            (seq_t)1   * memo[(size_t)(i-12)];

        memo[(size_t)i] = pos - neg;
    }
    computed_up_to = n;
}

seq_t a(idx_t n)
{
    if (n <= 12) return init[(size_t)n];

    ensure_memo(n);
    if (memo == NULL) return (seq_t)0;   /* allocation failure */
    return memo[(size_t)n];
}
