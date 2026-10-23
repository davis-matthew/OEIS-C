/*
 * A123920  Number of numbers congruent to 2 or 4 mod 6 between n and 2n
 *          inclusive.
 * https://oeis.org/A123920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integers k with n ≤ k ≤ 2n such that k ≡ 2 or 4
 *            (mod 6)
 * Range:     u8 n <= 167, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count numbers ≤ M congruent to r (mod 6), assuming r ∈ {0,1,2,3,4,5}. */
static inline seq_t count_upto(seq_t M, unsigned r)
{
    if (M < r)
        return 0;
    return ((M - r) / 6) + 1;
}

seq_t a(idx_t n)
{
    seq_t N        = (seq_t)n;        /* convert to seq_t for safety */
    seq_t twoN     = N * 2;           /* 2n */
    seq_t nMinus1  = N - 1;           /* n-1 (n ≥ 1) */

    seq_t c2 = count_upto(twoN, 2) - count_upto(nMinus1, 2);
    seq_t c4 = count_upto(twoN, 4) - count_upto(nMinus1, 4);

    return c2 + c4;
}
