/*
 * A067472  Smallest n-digit square starting with 2.
 * https://oeis.org/A067472
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = ceil(sqrt(2*10^(n-1)))^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   bitlen_seq_t, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/bitlen_seq_t.h"

/* Compute the integer ceil(sqrt(v)) by binary search.
   Uses bit length to obtain an upper bound that surely exceeds the root. */
static seq_t ceil_sqrt_seq_custom(seq_t v)
{
    if (v <= 1)
        return v;

    /* high is 1 << ceil(bitlen(v)/2)  – guarantees high*high >= v */
    unsigned int bits = bitlen_seq_t(v);
    seq_t high = ((seq_t)1) << ((bits + 1) / 2);
    seq_t low  = 0;

    while (low < high) {
        seq_t mid = low + (high - low) / 2;
        seq_t sq  = mid * mid;
        if (sq >= v)
            high = mid;
        else
            low  = mid + 1;
    }
    return low;
}

/* OEIS sequence A067472: smallest n‑digit square starting with 2. */
seq_t a(idx_t n)
{
    if (n < 2)               /* sequence is defined only for n ≥ 2 */
        return (seq_t)0;

    /* v = 2 * 10^(n-1)  */
    seq_t v = (seq_t)2 * pow10((idx_t)(n - 1));

    /* m = ceil(sqrt(v))  */
    seq_t m = ceil_sqrt_seq_custom(v);

    /* return m^2  */
    return m * m;
}
