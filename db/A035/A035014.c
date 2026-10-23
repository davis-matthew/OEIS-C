/*
 * A035014  a(n) contains n digits (either '3' or '4') and is divisible by
 *          2^n.
 * https://oeis.org/A035014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n-1) + 10^(n-1)*(4 - (a(n-1)/2^(n-1) mod 2)) Initial
 *            value: a(1) = 4 The sequence is memoized and can be called in
 *            any order
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* out of bounds – not used, but defined for safety */
    if (n < 1)
        return (seq_t)0;

    /* static cache state ------------------------------------------------- */
    static seq_t *seq   = NULL;     /* 1‑based array of computed terms */
    static size_t seq_cap = 0;      /* current capacity of the array     */
    static size_t seq_len = 0;      /* largest index already computed    */

    /* static powers used in the recurrence */
    static seq_t pow10_cur = 0;     /* 10^(i-1) for the next computation */
    static seq_t pow2_cur  = 0;     /* 2^i  for the next computation     */

    /* make sure we have space up to n ------------------------ */
    if (seq_cap <= (size_t)n) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* initialise for the first call ------------------------- */
    if (seq_len == 0) {
        seq[1] = (seq_t)4;          /* a(1) */
        seq_len = 1;
        pow10_cur = (seq_t)10;      /* 10^(2-1) for the first step of the loop */
        pow2_cur  = (seq_t)2;       /* 2^1, the power needed for checking a(2)   */
    }

    /* compute missing terms up to n ------------------------- */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        /* test whether a(i-1) is divisible by 2^i */
        seq_t two_i = pow2_cur * (seq_t)2;          /* 2^i */
        seq_t rem   = seq[i-1] % two_i;           /* a(i-1) mod 2^i */
        seq_t digit = (rem == (seq_t)0) ? (seq_t)4 : (seq_t)3;

        /* a(i) = a(i-1) + digit * 10^(i-1) */
        seq[i] = seq[i-1] + pow10_cur * digit;

        /* advance the powers for the next iteration */
        pow10_cur *= (seq_t)10;
        pow2_cur  *= (seq_t)2;
    }

    seq_len = n;
    return seq[n];
}
