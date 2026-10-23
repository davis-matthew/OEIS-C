/*
 * A171758  Binary expansion of numbers in A171757.
 * https://oeis.org/A171758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A171758 (binary expansion of numbers in A171757).
   For n >= 1, let L be the smallest integer such that 2^(L-1) >= n.
   For n > 1:
        offset = n - 2^(L-2)
        c      = 2^(L-1) + offset - 1          // = A171757(n)/2
        b      = 2 * c                         // = A171757(n)
   a(n) is obtained by interpreting the binary digits of b as decimal digits. */
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* Handle n==0 (outside offset) safely. */
    if (n == 0)
        return 0;

    /* Special case n == 1 (L == 1) */
    if (n == 1)
        return (seq_t)10;

    /* General case */
    idx_t m = n - 1;                    /* n-1 >= 1 */
    unsigned L = floor_log2_uint64(m) + 2;   /* L >= 2 */

    idx_t prev_total = ((idx_t)1) << (L - 2);   /* number of terms before length L */
    idx_t offset = n - prev_total;              /* 1 ≤ offset ≤ 2^(L-2) */

    idx_t c = ((idx_t)1) << (L - 1);            /* leading '1' bit */
    c += offset - 1;                            /* lower L-1 bits */

    seq_t b = ((seq_t)c) << 1;                  /* A171757(n) = 2*c */

    /* Convert binary digits of b into decimal digits. */
    seq_t res = 0;
    int high = (int)floor_log2_uint64((idx_t)b);
    for (int i = high; i >= 0; --i) {
        seq_t bit = (b >> i) & 1;
        res = res * 10 + bit;
    }
    return res;
}
