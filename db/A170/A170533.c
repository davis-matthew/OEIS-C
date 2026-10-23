/*
 * A170533  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 42*a(n-1)+42*a(n-2)+…+42*a(n-45) – 903*a(n-46) for n ≥ 46
 *            The first 46 values are obtained from the recurrence together
 *            with the adjustment term p(n) coming from the numerator of the
 *            rational generating function. This implementation memoises
 *            results in a static array so that repeated calls...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_N 20000        /* should be ≥ the largest n expected (≈10 000) */

static seq_t cache[MAX_N] = {0};   /* memoised values, zeroed at start */
static idx_t max_computed = 0;     /* largest index already computed */
static bool initialized = false;   /* a(0) set once */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Initialise the base value a(0) only once. */
    if (!initialized) {
        cache[0] = (seq_t)1;
        initialized = true;
    }

    /* Return already‑computed value if possible. */
    if (n <= max_computed)
        return cache[n];

    /* Compute consecutive terms up to n (order‑46 linear recurrence). */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t sum = 0;

        /* 42 * a(k-i) for i = 1..45, skipping negative indices. */
        for (idx_t i = 1; i <= 45 && k >= i; ++i) {
            sum += cache[k - i] * (seq_t)42U;
        }

        /* -903 * a(k-46) when k ≥ 46. */
        if (k >= 46) {
            sum -= cache[k - 46] * (seq_t)903U;
        }

        /* Adjustment term p(k) derived from the numerator of the GF. */
        seq_t p = 0;
        if (k == 0) {
            p = (seq_t)1;
        } else if (k <= 45) {
            p = (seq_t)2;
        } else if (k == 46) {
            p = (seq_t)1;
        } /* for k > 46, p remains 0 */

        sum += p;
        cache[k] = sum;
    }

    max_computed = n;
    return cache[n];
}
