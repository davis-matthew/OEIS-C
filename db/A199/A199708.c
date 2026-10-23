/*
 * A199708  Number of -n..n arrays x(0..5) of 6 elements with zero sum and no
 *          two neighbors equal.
 * https://oeis.org/A199708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + 2*a(n-5) - a(n-6) - a(n-7)
 *            + 2*a(n-8) - a(n-10) - 2*a(n-11) + 3*a(n-12) - a(n-13) with
 *            a(1)=32, a(2)=708, a(3)=4964, a(4)=20116, a(5)=59992,
 *            a(6)=147072, a(7)=314532, a(8)=608420, a(9)=1089752,
 *            a(10)=1836600, a(11)=2946204, a(12)=4537152, a(13)=6751384
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 47, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *memo = NULL;   /* 1‑indexed cache, memo[0] unused */
static size_t memo_size = 0; /* current cache size (indexed from 0) */

static const seq_t init_vals[13] = {
    32ULL, 708ULL, 4964ULL, 20116ULL, 59992ULL,
    147072ULL, 314532ULL, 608420ULL, 1089752ULL,
    1836600ULL, 2946204ULL, 4537152ULL, 6751384ULL
};

seq_t a(idx_t n)
{
    /* Undefined for n < 1, return 0 but legal calls start at 1. */
    if (n < 1) return 0;

    size_t idx = (size_t)n;
    /* Ensure cache large enough */
    if (memo == NULL || idx + 1 > memo_size) {
        size_t new_size = memo_size;
        if (new_size == 0) new_size = 14;                  /* room for 1..13 */
        if (idx + 1 > new_size) new_size = idx + 1;

        seq_t *tmp = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;                         /* allocation fail */
        memo = tmp;
        memo_size = new_size;

        /* Initialise the fixed first 13 values whenever the cache grows. */
        for (size_t i = 1; i <= 13; ++i) memo[i] = init_vals[i-1];
    }

    /* Parameters 1..13 already set – just return if requested. */
    if (idx <= 13) return memo[idx];

    /* Compute any missing values up to the requested index. */
    for (size_t i = 14; i <= idx; ++i) {
        seq_t pos = 0, neg = 0, tmp;

        /* Positive contributions */
        tmp = memo[i-1];
        tmp += tmp;         /* 2*a(i-1) */
        tmp += memo[i-1];    /* 3*a(i-1) */
        pos += tmp;

        tmp = memo[i-5];
        tmp += tmp;         /* 2*a(i-5) */
        pos += tmp;

        tmp = memo[i-8];
        tmp += tmp;         /* 2*a(i-8) */
        pos += tmp;

        tmp = memo[i-12];
        tmp += tmp;         /* 2*a(i-12) */
        tmp += memo[i-12];   /* 3*a(i-12) */
        pos += tmp;

        /* Negative contributions */
        tmp = memo[i-2];
        tmp += tmp;         /* 2*a(i-2) */
        neg += tmp;

        neg += memo[i-3];   /* a(i-3) */
        neg += memo[i-6];   /* a(i-6) */
        neg += memo[i-7];   /* a(i-7) */
        neg += memo[i-10];  /* a(i-10) */

        tmp = memo[i-11];
        tmp += tmp;         /* 2*a(i-11) */
        neg += tmp;

        neg += memo[i-13];  /* a(i-13) */

        memo[i] = pos - neg;
    }

    return memo[idx];
}
