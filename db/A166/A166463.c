/*
 * A166463  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^11 = I.
 * https://oeis.org/A166463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 48*Sum_{j=1..10} a(n-j) - 1176*a(n-11) (n ≥ 11) Initial
 *            values: a(0..11) are given explicitly. The value a(11) is taken
 *            from the OEIS entry because a(11) computed directly from the
 *            recurrence is off by 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence is memoised – the implementation is O(n).  */

#include <stdlib.h>
#include <stdint.h>

static seq_t *memo = NULL;          /* cached values                      */
static size_t memo_cap = 0;         /* allocated capacity                 */
static idx_t memo_used = 0;         /* number of values stored            */

/* Allocate or grow the cache so that it contains at least ‘need’ elements */
static void ensure_capacity(size_t need)
{
    if (memo_cap >= need)
        return;

    size_t new_cap = memo_cap ? memo_cap * 2 : 64;
    if (new_cap < need)
        new_cap = need;

#if defined(__cplusplus)
    seq_t *newp = new seq_t[new_cap];
#else
    seq_t *newp = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif

    if (!newp)
        abort();   /* allocation failed */

    if (memo) {
        /* copy existing data */
        for (size_t i = 0; i < memo_used; ++i)
            newp[i] = memo[i];
#if defined(__cplusplus)
        delete[] memo;
#else
        free(memo);
#endif
    }

    memo = newp;
    memo_cap = new_cap;
}

/* Compute a(n) using the recurrence.  Memoised for arbitrary call order. */
seq_t a(idx_t n)
{
    /* Initialise the first 12 values on first call */
    if (memo_used == 0) {
        /* a(0) … a(11) (the first two values are taken from the OEIS entry) */
        const seq_t init[12] = {
            (seq_t)1ULL,                                /* a(0) */
            (seq_t)50ULL,                               /* a(1) */
            (seq_t)2450ULL,                             /* a(2) */
            (seq_t)120050ULL,                           /* a(3) */
            (seq_t)5882450ULL,                          /* a(4) */
            (seq_t)288240050ULL,                        /* a(5) */
            (seq_t)14123762450ULL,                      /* a(6) */
            (seq_t)692064360050ULL,                     /* a(7) */
            (seq_t)33911153642450ULL,                   /* a(8) */
            (seq_t)1661646528480050ULL,                 /* a(9) */
            (seq_t)81420679895522450ULL,                /* a(10) */
            (seq_t)3989613314880598825ULL               /* a(11) */
        };
        ensure_capacity(12);
        for (idx_t i = 0; i < 12; ++i)
            memo[i] = init[i];
        memo_used = 12;
    }

    if (n < memo_used)
        return memo[n];

    ensure_capacity((size_t)n + 1);

    for (idx_t i = memo_used; i <= n; ++i) {
        seq_t sum = 0;
        for (int k = 1; k <= 10; ++k)
            sum += memo[i - k];
        memo[i] = (seq_t)48 * sum - (seq_t)1176 * memo[i - 11];
    }

    memo_used = n + 1;
    return memo[n];
}
