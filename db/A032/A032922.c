/*
 * A032922  Numbers whose set of base-15 digits is {1,3}.
 * https://oeis.org/A032922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 766, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th number whose base‑15 representation uses only digits 1 and 3.
   For each integer L ≥ 1 all 2^L such length‑L numbers are listed in lexicographic
   order, and the sequence is the concatenation of these blocks for increasing L.
   Algorithm:
       1. Find the length L such that the cumulative count of numbers of length
          < L is less than n and the cumulative count up to length L is ≥ n.
          The cumulative count up to length L‑1 is cum = Σ_{i=1}^{L-1} 2^i = 2^L−2.
       2. k = n − cum − 1 gives the 0‑based index of the desired number within
          the block of length L.
       3. Interpret k as an L‑bit binary number; treat bit 0 as digit 1 and bit 1
          as digit 3. Build the value by repeated multiplication by 15 and
          addition of the digit.
   Complexity is O(log n) and uses only integer arithmetic. */
seq_t a(idx_t n)
{
    idx_t cum   = 0;           /* sum of counts for lengths < L      */
    idx_t power = (idx_t)2;   /* 2^1   */
    idx_t L     = 1;           /* current length                     */

    /* Find the length L where the desired index lies. */
    while (cum + power < n)
    {
        cum += power;         /* cum becomes Σ_{i=1}^{L} 2^i       */
        power <<= 1;          /* next power of two = 2^{L+1}       */
        ++L;                  /* increase length                   */
    }

    /* k is the 0‑based index inside the block of length L. */
    idx_t k = n - cum - 1;

    seq_t val = 0;
    for (idx_t i = 0; i < L; ++i)
    {
        idx_t shift = L - 1 - i;        /* position from most significant */
        idx_t bit   = (k >> shift) & (idx_t)1;
        seq_t digit = (seq_t)(bit ? 3 : 1);
        val = val * 15 + digit;
    }
    return val;
}
