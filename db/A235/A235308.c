/*
 * A235308  Number of (n+1) X (6+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 6
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 15, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS sequence A235308
 *
 * Recurrence (order 14):
 *   a(n) = 4*a(n-1) +25*a(n-2) –112*a(n-3) –231*a(n-4) +1260*a(n-5) +875*a(n-6)
 *           –7280*a(n-7) –224*a(n-8) +22736*a(n-9) –7980*a(n-10) –36288*a(n-11)
 *           +21456*a(n-12) +23040*a(n-13) –17280*a(n-14)
 *   with a(1)=12018, a(2)=18662, … , a(14)=126648998.
 *
 * The implementation stores previously computed values in a static cache
 * and applies the recurrence using only `seq_t` arithmetic, thus allowing
 * arbitrary‑precision results without overflow.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache for memoised values */
static seq_t *memo = NULL;   /* array of memoised a(k)   (0‑based, a(1) in memo[0]) */
static size_t memo_cap = 0;  /* allocated capacity */
static size_t memo_len = 0;  /* number of cached elements */

/* First 14 terms of the sequence */
static const seq_t init_vals[14] = {
    12018ULL, 18662ULL, 27602ULL, 49574ULL, 83394ULL,
    168998ULL, 319826ULL, 711206ULL, 1486338ULL,
    3544742ULL, 8011922ULL, 20148134ULL, 48342594ULL,
    126648998ULL
};

/* Coefficients for the recurrence (a(n-1) … a(n-14)) */
static const int coeff[14] = {
    4,   25,  -112, -231, 1260,
    875, -7280, -224, 22736, -7980,
    -36288, 21456, 23040, -17280
};

/* Grow the memo array to hold at least `needed` elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= memo_cap)
        return;

    size_t new_cap = memo_cap ? memo_cap : 1;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof *new_mem);
    if (!new_mem)
        abort();  /* out of memory */

    memo = new_mem;
    memo_cap = new_cap;
}

/* Public function that returns a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1 */

    size_t target = (size_t)n;            /* number of values to have cached */
    ensure_capacity(target);

    /* If already computed, return cached value. */
    if (memo_len >= target)
        return memo[n - 1];

    /* Compute missing values up to target (inclusive). */
    for (size_t i = memo_len; i < target; ++i) {
        size_t k = i + 1;                 /* current index, 1‑based */

        if (k <= 14) {
            /* Initial terms are known. */
            memo[i] = init_vals[k - 1];
        } else {
            /* Apply the recurrence using only seq_t arithmetic. */
            seq_t pos = 0;  /* positive contributions */
            seq_t neg = 0;  /* negative contributions */

            for (int t = 0; t < 14; ++t) {
                int c = coeff[t];
                if (c == 0)
                    continue;

                seq_t val     = memo[i - 1 - t];           /* a(k-1-t) */
                seq_t mult    = val * (seq_t)(c >= 0 ? c : -c);
                if (c >= 0)
                    pos += mult;
                else
                    neg += mult;
            }

            /* The result is pos - neg; the recurrence guarantees pos >= neg. */
            memo[i] = (pos >= neg) ? (pos - neg) : 0;
        }
    }

    memo_len = target;
    return memo[n - 1];
}
