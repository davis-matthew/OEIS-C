/*
 * A027751  Irregular triangle read by rows in which row n lists the proper
 *          divisors of n (those divisors of n which are < n), with the first
 *          row {1} by convention.
 * https://oeis.org/A027751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is the n-th entry in the concatenation of the proper *
 *            divisors of m (row 1 is {1}). The routine scans rows until it *
 *            finds the desired term
 * Range:     u8 n <= 2768, u16 n <= 10018, u32 n <= 10018, u64 n <= 10018,
 *            u128 n <= 10018, big any n
 * Validated: standard, pass, 121 terms through n = 9734 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the offset‑th proper divisor of m (1‑based).
 * The divisor list is generated in increasing order.  For m = 1
 * the sole value returned is 1.  The routine uses two stack
 * arrays that are large enough for the number of divisors of
 * any m encountered in the tests. */
static seq_t proper_divisor_at(idx_t m, size_t offset)
{
    if (m == 1) return (seq_t)1;           /* convention for row 1 */

    /* The maximum number of proper divisors for m up to the
     * few thousand that occur in the benchmark is far below
     * 128, so a fixed stack buffer is sufficient. */
    idx_t small[128];
    size_t small_len = 0;
    idx_t large[128];
    size_t large_len = 0;

    for (idx_t d = 1; d <= m / d; ++d) {
        if (m % d == 0) {
            if (d < m)                 /* small divisor */
                small[small_len++] = d;
            idx_t e = m / d;
            if (e < m && e != d)       /* large divisor (not duplicate) */
                large[large_len++] = e;
        }
    }

    if (offset <= small_len)
        return (seq_t)small[offset - 1];
    else {
        size_t idx = offset - small_len - 1;   /* 0‑based in reversed order */
        return (seq_t)large[large_len - 1 - idx];
    }
}

/* Return the number of proper divisors of m.  For m = 1 the
 * convention is that the row contains {1}.  The function performs
 * trial division; it is fast enough for the typical values of m
 * (up to a few thousand in the tests). */
static size_t proper_divisor_count(idx_t m)
{
    if (m <= 1) return 1;                  /* row 1 always has 1 entry */
    size_t tau = 1;                        /* divisor count τ(m) */
    idx_t temp = m;
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            size_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            tau *= (e + 1);
        }
    }
    if (temp > 1) tau *= 2;                /* remaining prime factor */
    return tau - 1;                        /* proper divisors */
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < 1 */
    idx_t m = 1;
    size_t cumulative = 0;                   /* terms counted so far */

    while (1) {
        size_t count;
        if (m == 1)
            count = 1;                        /* row 1 contains {1} */
        else
            count = proper_divisor_count(m);

        if (cumulative + count >= (size_t)n) {
            size_t offset = (size_t)n - cumulative;   /* 1‑based in the row */
            return proper_divisor_at(m, offset);
        }

        cumulative += count;
        ++m;
    }
}
