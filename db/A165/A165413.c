/*
 * A165413  a(n) is the number of distinct lengths of runs in the binary
 *          representation of n.
 * https://oeis.org/A165413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of distinct run lengths in the binary
 *            representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count set bits in a 64‑bit word */
static int popcount_uint64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    /* Portable fallback */
    int c = 0;
    while (x) {
        x &= x - 1;
        c++;
    }
    return c;
#endif
}

/*
 * a(n) : number of distinct run lengths in the binary representation of n
 * Input: n >= 1 (idx_t is unsigned 64‑bit)
 * Output: seq_t value
 */
seq_t a(idx_t n)
{
    if (n == 0)        /* sequence starts at n=1, but handle gracefully */
        return (seq_t)0;

    /* Find highest set bit position */
    int msb = 63;
    while (((uint64_t)1 << msb) > n) msb--;

    uint64_t visited = 0;          /* bit i set => run length i+1 seen */
    int curr_bit = (n >> msb) & 1;
    int run_len  = 1;              /* current run length */

    /* Scan bits from msb-1 down to 0 */
    for (int i = msb - 1; i >= 0; --i) {
        int bit = (n >> i) & 1;
        if (bit == curr_bit) {
            run_len++;
        } else {
            /* record previous run length */
            visited |= (uint64_t)1 << (run_len - 1);
            curr_bit = bit;
            run_len = 1;
        }
    }
    /* record final run length */
    visited |= (uint64_t)1 << (run_len - 1);

    /* Count distinct lengths */
    return (seq_t)popcount_uint64(visited);
}
