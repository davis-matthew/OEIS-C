/*
 * A023721  Numbers with no 0's in their base-5 expansion.
 * https://oeis.org/A023721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = numbers with no 0's in their base-5 expansion
 * Range:     u8 n <= 148, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* All arguments use the integral types supplied by the build.
   The function returns the nth term (1‑based) of OEIS sequence A023721. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Return the n-th term of sequence A023721 */
seq_t a(idx_t n)
{
    /* undefined below offset, but return 0 for safety */
    if (n < 1u) return (seq_t)0;

    /* Find the number of base‑5 digits k of a(n).  The cumulative count
       after k digits is S(k) = Σ_{i=1..k} 4^i = (4^(k+1)-4)/3.  We
       find the smallest k with S(k) ≥ n.  All intermediate arithmetic
       uses __uint128_t to avoid overflow.  */
    uint64_t k = 1;                 /* current length (at least 1) */
    __uint128_t p = 4;              /* 4^k  (k=1) */
    __uint128_t cum = 0;            /* Σ_{i=1..k-1} 4^i  (zero when k=1) */

    while (cum + p < n) {
        cum += p;                  /* accumulate 4^(k) */
        p *= 4;                    /* next power 4^(k+1) */
        ++k;
    }

    /* At this point:  k = length of a(n) in base‑5,
       p   = 4^k,
       cum = Σ_{i=1..k-1} 4^i   (the count of all terms with fewer digits). */

    /* Offset within the block of length k numbers (0‑based). */
    idx_t offset = n - (idx_t)cum - 1u;      /* fits in idx_t (≤ 2^64-1) */

    /* Compute the value of a(n) by interpreting the offset as a base‑4
       number with exactly k digits and mapping each digit d (0–3)
       to base‑5 digit (d+1).  The computation is performed in base‑5. */
    seq_t val = (seq_t)0;
    __uint128_t pow = p / 4;                 /* 4^(k‑1)  (largest power of 4) */

    for (uint64_t i = 0; i < k; ++i) {
        uint64_t digit = (uint64_t)(offset / pow);   /* digit 0..3 */
        offset -= digit * (uint64_t)pow;             /* remove this digit */
        pow /= 4;                                   /* next smaller power */
        val = val * 5 + (seq_t)(digit + 1u);         /* append base‑5 digit */
    }

    return val;
}
