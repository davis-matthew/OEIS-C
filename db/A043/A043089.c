/*
 * A043089  Every string of 2 consecutive base-3 digits contains exactly 2
 *          distinct numbers.
 * https://oeis.org/A043089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th smallest nonnegative integer whose base‑3
 * representation has no equal adjacent digits.  Numbers with L digits
 * are in one‑to‑one correspondence with the L‑bit integers 0 … 2^L‑1.
 * The code determines the length L of the n‑th number, then
 * reconstructs the base‑3 digits from the binary encoding. */
seq_t a(idx_t n)
{
    /* Special case n == 0 → value 0. */
    if (n == 0)
        return (seq_t)0;

    /* Compute L such that 2^(L+1) >= n + 2.   L >= 1 for n >= 1. */
    idx_t x = n + 2;          /* x > 1  (since n>=1) */
    int L1 = 0;
    while (((idx_t)1 << L1) < x)
        ++L1;
    int L = L1 - 1;           /* length in digits (>=1) */

    /* Count of numbers whose length < L.  For L==1 this is 0,
     * otherwise it is 2^L - 1. */
    idx_t prev_count = (L == 1) ? 0 : (((idx_t)1 << L) - 1);

    /* Index within the block of numbers of length L, zero‑based. */
    idx_t idx = n - prev_count;

    /* L == 1: digits are simply 0,1,2.  idx will be 1 or 2 for n>=1. */
    if (L == 1)
        return (seq_t)idx;

    /* For L >= 2 build the number digit by digit. */
    seq_t val = 0;
    int bit;
    int prev_digit, curr_digit;
    int first_bit = (idx >> (L - 1)) & 1;
    prev_digit = (first_bit == 0) ? 1 : 2;
    val = prev_digit;

    /* Helper table: choices[prev_digit][0] = first option,
     * choices[prev_digit][1] = second option, both sorted ascending. */
    static const unsigned char choices[3][2] = {
        {/*prev 0*/ 1, 2},
        {/*prev 1*/ 0, 2},
        {/*prev 2*/ 0, 1}
    };

    for (int pos = 1; pos < L; ++pos) {
        bit = (idx >> (L - 1 - pos)) & 1;
        curr_digit = (bit == 0) ? choices[prev_digit][0]
                                : choices[prev_digit][1];
        val = val * (seq_t)3 + (seq_t)curr_digit;
        prev_digit = curr_digit;
    }

    return val;
}
