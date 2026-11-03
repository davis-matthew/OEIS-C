/*
 * A126863  S(1)={1}. S(n) = {S(n-1) {1,2,...,(n-1),n,(n-1),...,2,1} S(n-1)},
 *          where S(n) is a string of the first 3*2^n -2*n -3 terms of the
 *          sequence.
 * https://oeis.org/A126863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6119, u16 n <= 6119, u32 n <= 6119, u64 n <= 6119,
 *            u128 n <= 6119, big any n
 * Validated: standard, pass, 121 terms through n = 5678 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126863 – a(n) is the n-th term of the infinite palindrome
   string constructed by S(1)= {1} and
   S(k) = S(k-1) ∘ [1,2,…,k,…,2,1] ∘ S(k-1).
   The formula used below follows from this recursive definition. */

#include <stdint.h>

static seq_t seq_a_recurse(idx_t pos)
{
    /* Find the smallest level L such that pos ≤ L_L where
       L_k = 3·2^k – 2k – 3  (length of S(k)). */
    idx_t level = 1;          /* k */
    idx_t L = 1;              /* L_1 */
    idx_t pow2 = 2;           /* 2^1 */

    while (pos > L) {
        ++level;
        pow2 <<= 1;           /* 2^level */
        L = 3ULL * pow2 - 2ULL * level - 3ULL;
    }

    if (level == 1)           /* S(1) = {1} */
        return (seq_t)1;

    /* Length of the previous level S(level‑1) */
    idx_t pow2prev = pow2 >> 1; /* 2^(level-1) */
    idx_t Lprev = 3ULL * pow2prev - 2ULL * (level - 1) - 3ULL;
    idx_t mid_len = 2ULL * level - 1ULL; /* length of middle block */

    if (pos <= Lprev)          /* left copy of S(level‑1) */
        return seq_a_recurse(pos);

    if (pos <= Lprev + mid_len) {
        /* middle block: 1,2,…,level,…,2,1 */
        idx_t j = pos - Lprev;        /* 1‑based offset within middle */
        if (j <= level)
            return (seq_t)j;
        else
            return (seq_t)(2ULL * level - j);
    }

    /* right copy of S(level‑1) */
    pos -= (Lprev + mid_len);
    return seq_a_recurse(pos);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* out of defined range – not used */
    return seq_a_recurse(n);
}
