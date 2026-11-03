/*
 * A085015  Multiplicity of the root 1 in the characteristic polynomial mod 2
 *          of the n X n matrix with entries binomial(i+j,i), 0<=i,j<n.
 * https://oeis.org/A085015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0 a(2^l - k) = (2^l + 2*(-1)^l)/3 - k + 2*a(k) for 0 ≤ k
 *            ≤ 2^{l-1}, where l = ⌈log₂(n)⌉ and k = 2^l - n. The recursion is
 *            evaluated only for n > 0. It runs in O(log n) time and needs no
 *            global state. For n = 0 the value is 0
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 77 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for int64_t */

static seq_t a_recursive(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Find the smallest power of two ≥ n.  high = 2^l. */
    idx_t high = 1;
    int l = 0;                     /* l = log₂(high) after the loop */
    while (high < n) {
        high <<= 1;
        ++l;
    }

    /* k = 2^l – n, 0 ≤ k ≤ 2^{l‑1} */
    idx_t k = high - n;

    /* Recursive computation of a(k) */
    seq_t ak = a_recursive(k);

    /* (2^l + 2*(-1)^l) / 3   →  integer result */
    int64_t sign = (l & 1) ? -1 : 1;           /* (-1)^l  */
    int64_t temp = (int64_t)high + 2 * sign;   /* 2^l + 2*(-1)^l */
    seq_t term = (seq_t)(temp / 3);            /* exact division */

    /* Apply the recurrence: a(n) = term - k + 2*a(k) */
    seq_t result = term - (seq_t)k + (ak << 1);

    return result;
}

seq_t a(idx_t n)
{
    return a_recursive(n);
}
