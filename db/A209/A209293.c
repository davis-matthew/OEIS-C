/*
 * A209293  Inverse permutation of A185180.
 * https://oeis.org/A209293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (m1+m2‑1)*(m1+m2‑2)/2 + m1 where t = max{k : k(k+1)/2 <
 *            n} i = n – t(t+1)/2 (1 ≤ i ≤ t+1) j = t+2 – i m1 = ⌊(t+2)/2⌋ +
 *            ⌊i/2⌋·(–1)^{i+t+1} m2 = ⌊(t+3)/2⌋ + ⌊i/2⌋·(–1)^{i+t} (OEIS
 *            A209293, Inverse permutation of A185180)
 * Range:     u8 n <= 253, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* ---------- find t such that t*(t+1)/2 < n <= (t+1)*(t+2)/2  ---------- */
    idx_t low = 0, high = 1;
    /* grow high until it is greater than the desired t */
    while (( (__uint128_t)high * (high + 1) ) / 2 < n)
        high <<= 1;

    /* binary search for the largest t with t*(t+1)/2 < n */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (( (__uint128_t)mid * (mid + 1) ) / 2 < n)
            low = mid;
        else
            high = mid;
    }
    idx_t t = low;  /* t satisfies t*(t+1)/2 < n <= (t+1)*(t+2)/2 */

    /* ---------- compute i (and j, though j is not needed explicitly) ---------- */
    idx_t tri_t = (idx_t)(( (__uint128_t)t * (t + 1) ) / 2);
    idx_t i = n - tri_t;          /* 1 <= i <= t+1 */

    /* ---------- compute m1 and m2 ---------- */
    idx_t floor_i_div2 = i >> 1;   /* ⌊i/2⌋, always non‑negative */

    int64_t base1 = (int64_t)((t + 2) >> 1);   /* ⌊(t+2)/2⌋ */
    int64_t base2 = (int64_t)((t + 3) >> 1);   /* ⌊(t+3)/2⌋ */

    int sign1 = ((i + t + 1) & 1ULL) ? -1 : 1; /* (–1)^{i+t+1} */
    int sign2 = ((i + t) & 1ULL)     ? -1 : 1; /* (–1)^{i+t}   */

    int64_t m1_int = base1 + (int64_t)floor_i_div2 * sign1;
    int64_t m2_int = base2 + (int64_t)floor_i_div2 * sign2;

    /* ---------- convert to seq_t and evaluate the formula ---------- */
    seq_t m1 = (seq_t)m1_int;
    seq_t m2 = (seq_t)m2_int;
    seq_t sum = m1 + m2;          /* m1 + m2 */

    /* compute (sum-1)*(sum-2)/2 without overflow by dividing first */
    seq_t a_term = sum - 1;
    seq_t b_term = sum - 2;
    if (a_term & 1)               /* a_term is odd → divide b_term */
        b_term /= 2;
    else
        a_term /= 2;

    seq_t part = a_term * b_term;

    return part + m1;
}
