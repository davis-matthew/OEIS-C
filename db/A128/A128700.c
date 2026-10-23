/*
 * A128700  Highly abundant numbers with an odd divisor sum.
 * https://oeis.org/A128700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, full, nice, nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: compute the left‑to‑right list of highly abundant
 * numbers whose divisor sum σ(n) is odd.  The search stops
 * when the known upper bound 7200 is exceeded; the sequence
 * is known to have only 13 terms.
 *
 * The implementation precomputes σ(k) for 1 ≤ k ≤ 7200,
 * keeps the running maximum, and collects the numbers that
 * achieve a new maximum and satisfy the parity property.
 * Subsequent calls are answered in O(1) from the cached array.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- helper: integer square root ---------- */
static inline idx_t
isqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;           /* safe for n ≤ 7200 */
        if (sq == n)
            return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;   /* floor(sqrt(n)) */
}

/* test whether v is a perfect square */
static inline bool
is_square(idx_t v)
{
    idx_t r = isqrt(v);
    return r * r == v;
}

/* ---------- sequence cache ---------- */
static seq_t *sequence = NULL;   /* array of found terms */
static size_t seq_len  = 0;      /* number of terms stored */

/* initialise the sequence on first use */
static void
init_sequence(void)
{
    const idx_t LIMIT = 7200;                /* known upper bound */
    /* allocate array for divisor sums */
    seq_t *sigma = (seq_t *)malloc((LIMIT + 1) * sizeof(seq_t));
    if (!sigma)
        return;                              /* allocation failure */

    /* compute σ(k) for all k ≤ LIMIT */
    for (idx_t i = 1; i <= LIMIT; ++i) {
        for (idx_t j = i; j <= LIMIT; j += i)
            sigma[j] += (seq_t)i;
    }

    /* temporary storage for the terms found (there are only 13) */
    seq_t *tmp  = (seq_t *)malloc(20 * sizeof(seq_t));
    size_t cnt  = 0;
    seq_t max_sigma = 0;

    for (idx_t n = 1; n <= LIMIT; ++n) {
        if (sigma[n] > max_sigma) {
            max_sigma = sigma[n];
            if (is_square(n) || ((n & 1) == 0 && is_square(n >> 1)))
                tmp[cnt++] = n;
        }
    }

    free(sigma);

    sequence = (seq_t *)malloc(cnt * sizeof(seq_t));
    if (!sequence) {
        free(tmp);
        return;                              /* allocation failure */
    }
    for (size_t i = 0; i < cnt; ++i)
        sequence[i] = tmp[i];
    seq_len = cnt;

    free(tmp);
}

/* ---------- required public routine ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n = 0 */

    if (!sequence)
        init_sequence();          /* lazy initialisation */

    if (n > seq_len)
        return 0;                 /* beyond the end of the sequence */

    return sequence[n - 1];
}
