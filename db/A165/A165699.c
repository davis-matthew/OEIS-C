/*
 * A165699  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) for n ≥ 11 satisfies a(n) = 44*a(n-1) - 989*a(n-9) +
 *            946*a(n-10). Initial values (n = 0 … 10) are taken directly from
 *            OEIS
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static const seq_t init_vals[11] = {
    (seq_t)1ULL,                      /* a(0) */
    (seq_t)45ULL,                     /* a(1) */
    (seq_t)1980ULL,                   /* a(2) */
    (seq_t)87120ULL,                  /* a(3) */
    (seq_t)3833280ULL,                /* a(4) */
    (seq_t)168664320ULL,              /* a(5) */
    (seq_t)7421230080ULL,             /* a(6) */
    (seq_t)326534123520ULL,           /* a(7) */
    (seq_t)14367501434880ULL,         /* a(8) */
    (seq_t)632170063133730ULL,        /* a(9) */
    (seq_t)27815482777840560ULL       /* a(10) */
};

seq_t a(idx_t n)
{
    /* Return the pre‑computed initial terms directly. */
    if (n <= 10)
        return init_vals[(size_t)n];

    /* Sliding window holding a(k-10)…a(k-1) for the current k. */
    seq_t window[10];
    size_t i;
    for (i = 0; i < 10; ++i)
        window[i] = init_vals[i + 1];   /* start with a(1)…a(10) */

    /* Compute from k = 11 up to k = n. */
    for (i = 11; i <= n; ++i) {
        /* a(i) = 44*a(i-1) - 989*a(i-9) + 946*a(i-10). */
        seq_t next = (seq_t)44 * window[9] - (seq_t)989 * window[1] + (seq_t)946 * window[0];

        /* Shift the window: drop a(i-10), shift left, and insert next. */
        for (size_t shift = 0; shift < 9; ++shift)
            window[shift] = window[shift + 1];
        window[9] = next;
    }

    return window[9];   /* a(n) now stored here */
}
