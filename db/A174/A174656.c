/*
 * A174656  a(n) counts strict partitions of n into powers of 3^t, 5^f and 7^s
 *          with positive exponents t, f and s.
 * https://oeis.org/A174656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 8103 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts strict partitions of n into distinct powers of 3, 5, and 7
   (powers 3^t, 5^f, 7^s, with t, f, s ≥ 1).  The function uses a
   simple recursive subset–sum search over at most 17 distinct powers,
   which is fast enough for n up to 10‑000 and works for all n ≥ 1. */
#include <stddef.h>
#include <stdbool.h>

/* Recursive helper: count subsets of powers[p]..powers[k-1] that sum to rem. */
static seq_t _count(const idx_t *powers, size_t k, size_t pos, idx_t rem)
{
    if (rem == 0)          /* empty subset – one way */
        return 1;
    if (pos == k)          /* no more powers to use */
        return 0;
    if (rem < powers[pos]) /* even the smallest remaining power is too big */
        return _count(powers, k, pos + 1, rem);

    /* Include or exclude the current power */
    return _count(powers, k, pos + 1, rem) +
           _count(powers, k, pos + 1, rem - powers[pos]);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for the offset region */

    /* Build all powers of 3, 5, and 7 that do not exceed n. */
    idx_t powers[32];
    size_t k = 0;

    for (idx_t v = 3; v <= n; ) {
        powers[k++] = v;
        if (v > n / 3) break;
        v *= 3;
    }
    for (idx_t v = 5; v <= n; ) {
        powers[k++] = v;
        if (v > n / 5) break;
        v *= 5;
    }
    for (idx_t v = 7; v <= n; ) {
        powers[k++] = v;
        if (v > n / 7) break;
        v *= 7;
    }

    /* Quick rejection if n is larger than the sum of all powers. */
    idx_t total = 0;
    for (size_t i = 0; i < k; ++i) total += powers[i];
    if (total < n) return 0;

    return _count(powers, k, 0, n);
}
