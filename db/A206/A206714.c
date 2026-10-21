/*
 * A206714  Number of solutions (n,k) of s(k)=s(n) (mod n), where 1<=k<n and
 *          s(k)=2^(k-1).
 * https://oeis.org/A206714
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = #{ k | 1 ≤ k < n , 2^(k-1) ≡ 2^(n-1) (mod n) } *
 *            Method: compute 2^(n-1) mod n once, then count how many k have
 *            the * same value of 2^(k-1) mod n. Complexity: O(n) per call,
 *            fast for n ≤ 10000
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* for size_t */

seq_t a(idx_t n)
{
    if (n < 2u)          /* sequence is defined from n ≥ 2 */
        return (seq_t)0;

    /* First compute target = 2^(n-1) mod n */
    idx_t target = 1 % n;           /* 2^(1-1) mod n */
    for (idx_t i = 1; i < n; ++i)   /* iterate k = 2..n-1 to reach 2^(n-1) */
        target = (target * 2u) % n;

    /* Now count k with the same residue */
    idx_t curr = 1 % n;   /* 2^(1-1) mod n  */
    seq_t cnt = 0;
    for (idx_t k = 1; k < n; ++k) {
        if (curr == target)
            ++cnt;
        curr = (curr * 2u) % n;
    }
    return cnt;
}
