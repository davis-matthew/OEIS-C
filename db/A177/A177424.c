/*
 * A177424  Exponent of the highest power of 2 dividing binomial(n^2,n).
 * https://oeis.org/A177424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   v2(C(n^2,n)) = popcount_2(n) + popcount_2(n^2-n) -
 *            popcount_2(n^2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return: exponent of the highest power of 2 dividing binomial(n^2,n) */
seq_t a(idx_t n) {
    if (n == 0) return 0;        /* binomial(0,0)=1, exponent 0 */

    uint64_t nn = (uint64_t)n;

    /* popcount of n */
    unsigned pc_n = __builtin_popcountll(nn);

    /* n^2 and n*(n-1) use 128‑bit integers to avoid overflow */
    unsigned __int128 prod      = (unsigned __int128)nn * nn;
    unsigned __int128 prod_sub  = (unsigned __int128)nn * (nn - 1);

    /* popcount of 128‑bit numbers split into two 64‑bit halves */
    uint64_t prod_low  = (uint64_t)prod;
    uint64_t prod_high = (uint64_t)(prod >> 64);
    unsigned pc_prod   = __builtin_popcountll(prod_low) + __builtin_popcountll(prod_high);

    uint64_t sub_low  = (uint64_t)prod_sub;
    uint64_t sub_high = (uint64_t)(prod_sub >> 64);
    unsigned pc_sub   = __builtin_popcountll(sub_low) + __builtin_popcountll(sub_high);

    /* exponent = pc_n + pc_sub - pc_prod */
    return (seq_t)(pc_n + pc_sub - pc_prod);
}
