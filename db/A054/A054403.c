/*
 * A054403  Result of third stage of sieve of Eratosthenes (after eliminating
 *          multiples of 2, 3 and 5).
 * https://oeis.org/A054403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates the terms of OEIS A054403.
   The sequence consists of numbers ≥2 that are not divisible by 2, 3 or 5.
   Explicitly: 2, 3, 5, followed by all integers of the form
   30k + r where r ∈ {1,7,11,13,17,19,23,29} (k ≥ 1) and r=1 is not
   used for k = 0 because 1 < 2.  The following code returns the nth
   term (a(1) = 2) without precomputing a lookup table. */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;

    /* n ≥ 4: terms 7, 11, 13, ... */
    idx_t m = n - 4;            /* 0‑based index in the tail of the sequence */
    static const int pattern[8] = {1, 7, 11, 13, 17, 19, 23, 29};

    if (m < 7) {
        /* The first block (k = 0) contains 7 numbers: 7, 11, 13, 17, 19, 23, 29
           which correspond to pattern[1] … pattern[7] */
        return (seq_t)(pattern[m + 1]);
    }

    /* Subsequent blocks (k ≥ 1) contain 8 numbers per block */
    m -= 7;                     /* shift index to start from the first full block */
    idx_t k = m / 8;             /* block number (starting from 0) */
    idx_t offset = m % 8;        /* position within the block */
    return (seq_t)((seq_t)30 * (k + 1) + (seq_t)pattern[offset]);
}
