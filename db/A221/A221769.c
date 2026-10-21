/*
 * A221769  Number of n X 2 arrays of occupancy after each element stays put
 *          or moves to some horizontal or vertical neighbor, without move-in
 *          move-out left turns.
 * https://oeis.org/A221769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 67*a(n-2) + 120*a(n-3) - 80*a(n-4) +
 *            10*a(n-5) initial values: a(1)=3, a(2)=27, a(3)=240, a(4)=2127,
 *            a(5)=18828
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A221769 */

static seq_t cache[20001];        /* 1‑based cache, cache[0] unused */
static idx_t cache_last = 0;      /* largest index already computed */

/* a(n): number of n×2 arrays of occupancy … */
seq_t a(idx_t n)
{
    /* a(0) is defined as 0 for safety; the sequence is only for n≥1 */
    if (n == 0) return (seq_t)0;

    /* Ensure we do not exceed the fixed capacity (n ≤ 20000) */
    if (n >= 20001) {
        /* n is out of range; return zero (undefined behaviour beyond spec) */
        return (seq_t)0;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i == 1)          cache[i] = (seq_t)3;
        else if (i == 2)     cache[i] = (seq_t)27;
        else if (i == 3)     cache[i] = (seq_t)240;
        else if (i == 4)     cache[i] = (seq_t)2127;
        else if (i == 5)     cache[i] = (seq_t)18828;
        else {
            /* recurrence: a(n) = 15*a(n-1) - 67*a(n-2) + 120*a(n-3) - 80*a(n-4) + 10*a(n-5) */
            seq_t val = (seq_t)15 * cache[i-1];
            val -= (seq_t)67 * cache[i-2];
            val += (seq_t)120 * cache[i-3];
            val -= (seq_t)80 * cache[i-4];
            val += (seq_t)10 * cache[i-5];
            cache[i] = val;
        }
    }
    cache_last = n;
    return cache[n];
}
