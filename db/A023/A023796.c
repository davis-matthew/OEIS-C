/*
 * A023796  Katadromes: digits in base 15 are in strict descending order.
 * https://oeis.org/A023796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, easy
 * Formula:   a(n) = the n-th smallest integer whose digits in base 15 are in
 *            strictly decreasing order. The term is computed by enumerating
 *            all non‑empty subsets of {0,…,14}, interpreting each subset in
 *            decreasing digit order, sorting the resulting values, and
 *            selecting the (n‑1)‑th element (1‑indexed). The v...
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static int cmp_uint64(const void *p1, const void *p2)
{
    const uint64_t a = *(const uint64_t *)p1;
    const uint64_t b = *(const uint64_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* 2^15−1 = 32767 non‑empty subsets of {0,…,14}.  Store and sort once. */
    static bool initialized = false;
    static uint64_t values[1 << 15];   /* 32768 elements */
    static size_t count = 0;

    if (!initialized) {
        const size_t LIMIT = (size_t)1 << 15;   /* 32768 */
        size_t pos = 0;
        for (size_t mask = 1; mask < LIMIT; ++mask) {   /* skip empty subset */
            uint64_t val = 0;
            for (int d = 14; d >= 0; --d) {
                if (mask & ((size_t)1 << d))
                    val = val * 15 + d;
            }
            values[pos++] = val;
        }
        count = pos;                    /* should be 32767 */
        qsort(values, count, sizeof(uint64_t), cmp_uint64);
        initialized = true;
    }

    if (n < 1 || (size_t)n > count)
        return (seq_t)0;  /* out of range – undefined by OEIS */

    return (seq_t)values[(size_t)n - 1];
}
