/*
 * A220538  Equals one maps: number of 2 X n binary arrays indicating the
 *          locations of corresponding elements equal to exactly one of their
 *          horizontal, diagonal and antidiagonal neighbors in a random 0..1 2
 *          X n array.
 * https://oeis.org/A220538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   uses the empirical recurrence for n > 12: a(n) = 4*a(n-1) -
 *            4*a(n-2) + 3*a(n-3) - a(n-4) - 7*a(n-5) - 7*a(n-6) + 7*a(n-8) -
 *            6*a(n-9) with initial values: a(1)=1, a(2)=4, a(3)=5, a(4)=25,
 *            a(5)=55, a(6)=166, a(7)=446, a(8)=1240, a(9)=3377, a(10)=9220,
 *            a(11)=25086, a(12)=68225
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc, free */

static const seq_t base_vals[13] = {
    0,          /* unused index 0 */
    (seq_t)1,   /* a(1) */
    (seq_t)4,   /* a(2) */
    (seq_t)5,   /* a(3) */
    (seq_t)25,  /* a(4) */
    (seq_t)55,  /* a(5) */
    (seq_t)166, /* a(6) */
    (seq_t)446, /* a(7) */
    (seq_t)1240,/* a(8) */
    (seq_t)3377,/* a(9) */
    (seq_t)9220,/* a(10) */
    (seq_t)25086,/* a(11) */
    (seq_t)68225 /* a(12) */
};

static seq_t *memo = NULL;      /* memoized sequence values */
static idx_t memo_len = 0;      /* number of allocated entries (>=1) */

/* Ensure that memo has room for at least n+1 entries (index 0..n). */
static void ensure_memo(idx_t n)
{
    if (memo_len > n) return;
    idx_t new_len = memo_len ? memo_len * 2 : 16;
    while (new_len <= n) new_len <<= 1;
    seq_t *new_mem = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
    if (!new_mem) {  /* allocation failed – fatal */
        abort();
    }
    memo = new_mem;
    /* initialize new entries to zero; will be filled later */
    for (idx_t i = memo_len; i < new_len; ++i) memo[i] = (seq_t)0;
    memo_len = new_len;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined, but safe */

    ensure_memo(n);
    /* if the value is already computed, return it */
    if (memo[n] != (seq_t)0) return memo[n];

    /* fill from the smallest uncomputed index up to n */
    idx_t start = 1;
    for (idx_t i = 1; i < memo_len; ++i)
        if (memo[i] == (seq_t)0) { start = i; break; }
    if (start == 1 && memo[1] == (seq_t)0) start = 1; /* first call */

    for (idx_t i = start; i <= n; ++i) {
        seq_t val;
        if (i <= 12) {
            val = base_vals[i];
        } else {
            val = 4 * memo[i-1]      /* 4*a(i-1) */
                - 4 * memo[i-2]     /* -4*a(i-2) */
                + 3 * memo[i-3]     /* +3*a(i-3) */
                - memo[i-4]         /* -a(i-4) */
                - 7 * memo[i-5]     /* -7*a(i-5) */
                - 7 * memo[i-6]     /* -7*a(i-6) */
                + 7 * memo[i-8]     /* +7*a(i-8) */
                - 6 * memo[i-9];    /* -6*a(i-9) */
        }
        memo[i] = val;
    }
    return memo[n];
}
