/*
 * A120241  a(n) = (n - 2^floor(log(n)/log(2)) + 1)-th integer among those
 *          positive integers not among the earlier terms of the sequence.
 * https://oeis.org/A120241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = k-th smallest positive integer not already used, where k
 *            = n - 2^⌊log₂ n⌋ + 1 (A062050(n)). The sequence is a permutation
 *            of the positive integers
 * Range:     u8 n <= 191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   fenwick_init
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/fenwick_init.h"

/* Helper: return floor(log2(x)) for x >= 1 */
static inline unsigned floor_log2_u64(uint64_t x)
{
    /* __builtin_clzll is defined for non‑zero values */
    return 63 - __builtin_clzll(x);
}

static inline void fenwick_add(unsigned *bit, unsigned n, unsigned idx, int delta)
{
    while (idx <= n) {
        bit[idx] += delta;
        idx += idx & -idx;
    }
}

static inline unsigned fenwick_sum(const unsigned *bit, unsigned idx)
{
    unsigned res = 0;
    while (idx) {
        res += bit[idx];
        idx -= idx & -idx;
    }
    return res;
}

/* Find smallest index such that prefix sum >= k; k is 1‑based */
static unsigned fenwick_find_kth(const unsigned *bit, unsigned n, unsigned k)
{
    unsigned idx = 0;
    unsigned mask = 1u << (31 - __builtin_clz(n)); /* highest power of two <= n */
    while (mask) {
        unsigned t = idx + mask;
        if (t <= n && bit[t] < k) {
            idx = t;
            k -= bit[t];
        }
        mask >>= 1;
    }
    return idx + 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n=0; return 0 */

    /* Upper bound for values needed; conservatively 4*n + 10 */
    unsigned UB = (unsigned)(4 * n + 10);

    /* Fenwick tree array (1‑based); use dynamic allocation */
    unsigned *bit = (unsigned *)malloc((UB + 1) * sizeof(unsigned));
    if (!bit) return (seq_t)0;          /* allocation failure */

    fenwick_init(bit, UB);

    /* We don't need a separate 'used' array; BIT tracks unused counts */

    seq_t result = 0;

    for (unsigned i = 1; i <= (unsigned)n; ++i) {
        /* compute k = i - 2^floor(log2 i) + 1 */
        unsigned pw = 1u << floor_log2_u64(i);
        unsigned k = i - pw + 1;

        /* find k-th unused number */
        unsigned val = fenwick_find_kth(bit, UB, k);

        /* mark it used: subtract 1 at position val */
        fenwick_add(bit, UB, val, -1);

        if (i == (unsigned)n)
            result = (seq_t)val;
    }

    free(bit);
    return result;
}
