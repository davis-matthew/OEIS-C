/*
 * A134352  A130123 * A128174.
 * https://oeis.org/A134352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^r if (r - k) is even 0 otherwise where r =
 *            ⌊(√(8n+1)-1)/2⌋ (row number) k = n - r(r+1)/2 (column index
 *            within the row)
 * Range:     u8 n <= 35, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <type_traits>
#endif
#include <stdint.h>

/* Find the row number r for index n (0‑based). */
static inline idx_t row_of_n(idx_t n)
{
    idx_t low  = 0;
    idx_t high = n;        /* upper bound – safe for all n */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        __uint128_t t = (__uint128_t)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Compute 2^r as a seq_t, respecting truncation for fixed‑width types. */
static inline seq_t power_of_two(idx_t r)
{
#ifdef __cplusplus
    if (std::is_unsigned<seq_t>::value) {   /* built‑in unsigned integer types */
        constexpr idx_t bits = sizeof(seq_t) * 8;
        if (r >= bits) return (seq_t)0;
        return ((seq_t)1 << r);
    } else {             /* arbitrary‑precision integer class – shift is safe */
        return ((seq_t)1 << r);
    }
#else
    /* assume unsigned integer type */
    const idx_t bits = sizeof(seq_t) * 8;
    if (r >= bits) return (seq_t)0;
    return ((seq_t)1 << r);
#endif
}

seq_t a(idx_t n)
{
    idx_t r = row_of_n(n);
    __uint128_t tri = (__uint128_t)r * (r + 1) / 2;
    idx_t k = n - (idx_t)tri;          /* 0 ≤ k ≤ r */
    if (((r - k) & 1) == 0)
        return power_of_two(r);
    else
        return (seq_t)0;
}
