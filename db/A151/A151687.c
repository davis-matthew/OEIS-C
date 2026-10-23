/*
 * A151687  G.f.: x + x^2 * Product_{n>=0} (1 + x^(2^n-1) + x^(2^n)).
 * https://oeis.org/A151687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; a(2^k)=k+1; * for n≥3, if n=2^i + j with 1≤j<2^i then
 *            a(n)=a(j)+a(j+1)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* floor(log2(n)) for n>0 */
static inline idx_t floor_log2_idx(idx_t n)
{
    idx_t r = 0;
    while (n >>= 1) ++r;
    return r;
}

/* recursive computation of a(n) */
static seq_t get_val(idx_t n)
{
    if (n == 0) return 0;                      /* a(0) */
    if ((n & (n - 1)) == 0)                    /* power of two */
        return (seq_t)(floor_log2_idx(n) + 1);  /* a(2^k) = k+1 */

    idx_t i   = floor_log2_idx(n);             /* i with 2^i ≤ n < 2^(i+1) */
    idx_t pow = (idx_t)1 << i;                 /* 2^i */
    idx_t j   = n - pow;                       /* 1 ≤ j < 2^i */
    return get_val(j) + get_val(j + 1);        /* a(n)=a(j)+a(j+1) */
}

seq_t a(idx_t n)
{
    return get_val(n);
}
