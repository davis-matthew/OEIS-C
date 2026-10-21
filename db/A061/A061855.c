/*
 * A061855  Symmetric totally balanced binary sequences: those terms of
 *          A014486 which are equal to their reversed complement.
 * https://oeis.org/A061855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n_th smallest positive integer whose binary
 *            representation (without leading zeros) is a symmetric
 *            reversed‑complement binary sequence and is a Dyck word (never
 *            more closes than opens in any prefix)
 * Range:     u8 n <= 12, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: this implementation uses only standard integer arithmetic. */
#include <stdint.h>
#include <stdbool.h>

static bool initialized = false;

/* Pre‑computed tables */
static uint64_t dp[65][65];   /* dp[rem][bal] : number of good extensions */
static uint64_t cum[65];      /* cum[k] : total count for lengths up to k */

/* Reverse lower 'k' bits of 'x'. */
static uint64_t reverse_bits(uint64_t x, int k)
{
    uint64_t r = 0;
    for (int i = 0; i < k; ++i) {
        if (x & ((uint64_t)1 << i))
            r |= ((uint64_t)1 << (k - 1 - i));
    }
    return r;
}

/* Compute auxiliary tables once. */
static void init_tables(void)
{
    if (initialized) return;
    initialized = true;

    /* Base: empty string is valid for any starting balance. */
    for (int bal = 0; bal <= 64; ++bal)
        dp[0][bal] = 1;

    /* DP recurrence. */
    for (int rem = 1; rem <= 64; ++rem) {
        for (int bal = 0; bal <= 64; ++bal) {
            uint64_t val = 0;
            /* place '1'  -> increase balance */
            if (bal + 1 <= 64)
                val += dp[rem - 1][bal + 1];
            /* place '0'  -> decreased balance, allowed only if bal>0 */
            if (bal > 0)
                val += dp[rem - 1][bal - 1];
            dp[rem][bal] = val;
        }
    }

    /* cumulative counts */
    cum[0] = 0;
    for (int k = 1; k <= 64; ++k) {
        cum[k] = cum[k - 1] + dp[k - 1][1];
    }
}

seq_t a(idx_t n)
{
    init_tables();  /* ensure tables are ready */

    if (n == 0) return 0;   /* offset is 0 */

    /* Find the block (half‑length k) containing the n‑th term. */
    int k = 1;
    while (cum[k] < n) ++k;

    /* Offset inside current block (0‑based). */
    uint64_t offset = (uint64_t)n - cum[k - 1] - 1;

    /* Reconstruct the first half t of length k. */
    uint64_t t_val = ((uint64_t)1 << (k - 1));   /* first bit is always 1 */
    int bal = 1;                                 /* current balance after first bit */

    for (int pos = 1; pos < k; ++pos) {
        int rem = k - pos - 1;                   /* bits remaining after this position */

        uint64_t count0 = 0;
        if (bal > 0)
            count0 = dp[rem][bal - 1];           /* sequences starting with a 0 */

        if (offset < count0) {
            /* choose 0 at this position */
            bal--;
        } else {
            /* choose 1 at this position */
            offset -= count0;
            t_val |= ((uint64_t)1 << (k - 1 - pos));
            bal++;
        }
    }

    /* Build the full symmetric sequence s = t + complement(reverse(t)). */
    uint64_t rev = reverse_bits(t_val, k);
    uint64_t mask = ((uint64_t)1 << k) - 1;
    uint64_t low  = (~rev) & mask;
    uint64_t high = t_val << k;

    uint64_t result = high | low;
    return (seq_t)result;
}
