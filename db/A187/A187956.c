/*
 * A187956  Half the number of (n+2) X 3 binary arrays with no 3 X 3 subblock
 *          having a sum equal to any horizontal or vertical neighbor 3 X 3
 *          subblock sum.
 * https://oeis.org/A187956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n‑1) + 9*a(n‑2) – 36*a(n‑3) + 144*a(n‑4) + 324*a(n‑5)
 *            + 729*a(n‑6) – 2916*a(n‑7) – 6561*a(n‑8) Initial values (for 1 ≤
 *            n ≤ 8): a(1)=256, a(2)=1408, a(3)=7744, a(4)=42592, a(5)=240064,
 *            a(6)=1353088, a(7)=7626496, a(8)=42683776
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static const seq_t base[8] = {
        (seq_t)256ULL,
        (seq_t)1408ULL,
        (seq_t)7744ULL,
        (seq_t)42592ULL,
        (seq_t)240064ULL,
        (seq_t)1353088ULL,
        (seq_t)7626496ULL,
        (seq_t)42683776ULL
    };

    /* Limit for preallocated storage (sequence is guaranteed to be queried
       for n ≤ 10000 in the tests).  10005 leaves a small margin. */
    static seq_t cache[10005];
    static bool initialized = false;
    static size_t cached = 0; /* largest n for which cache[n] is valid */

    if (!initialized) {
        cache[0] = (seq_t)0;          /* unused but convenient for indices */
        for (size_t i = 0; i < 8; ++i)
            cache[i + 1] = base[i];
        cached = 8;
        initialized = true;
    }

    if (n <= cached)
        return cache[(size_t)n];

    for (size_t i = cached + 1; i <= (size_t)n; ++i) {
        seq_t val = (seq_t)4 * cache[i - 1];
        val += (seq_t)9 * cache[i - 2];
        val -= (seq_t)36 * cache[i - 3];
        val += (seq_t)144 * cache[i - 4];
        val += (seq_t)324 * cache[i - 5];
        val += (seq_t)729 * cache[i - 6];
        val -= (seq_t)2916 * cache[i - 7];
        val -= (seq_t)6561 * cache[i - 8];
        cache[i] = val;
    }
    cached = (size_t)n;
    return cache[(size_t)n];
}
