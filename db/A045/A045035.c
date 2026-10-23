/*
 * A045035  Numbers whose base-4 representation contains exactly four 0's and
 *          two 1's.
 * https://oeis.org/A045035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 935, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045035: Numbers whose base‑4 representation contains exactly
   four 0's and two 1's.  The algorithm enumerates all numbers of length
   L, ranks the nth one using combinatorial unranking, and returns the
   result. */

#include <stdint.h>
#include <stdbool.h>

/* 128‑bit type for intermediate combinatorial calculations.   */
typedef unsigned __int128 u128;

/* Binomial coefficient C(n,k) computed with 128‑bit arithmetic. */
static inline u128 comb128(unsigned int n, unsigned int k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;                 /* use symmetry */
    u128 res = 1;
    for (unsigned int i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

/* Number of strings of length `rem` containing `zeros` zeros,
   `ones` ones, and the remaining positions are 2 or 3 (two choices). */
static inline u128 remaining_count(unsigned int rem,
                                   unsigned int zeros,
                                   unsigned int ones)
{
    if (zeros > rem || ones > rem - zeros) return 0;
    u128 r = comb128(rem, zeros);
    r *= comb128(rem - zeros, ones);
    r <<= (rem - zeros - ones);              /* * 2^(rest) */
    return r;
}

/* Count of valid numbers of base‑4 length L (with exactly 4 zeros,
   2 ones, no leading zero). */
static inline u128 count_len(unsigned int L)
{
    if (L < 6) return 0;
    /* total arrangements for length L with 4 zeros and 2 ones */
    u128 base1      = comb128(L, 4);
    u128 base2      = comb128(L - 4, 2);
    u128 shift      = (u128)1 << (L - 6);

    /* subtract those whose most significant digit is 0 */
    u128 total      = (base1 - comb128(L - 1, 3)) * base2 * shift;
    return total;
}

/* Return the n‑th term (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* undefined, safe return */

    u128 cumulative = 0;                      /* total count of terms for  */
    unsigned int L   = 6;                       /* length currently examined */

    /* Find the length that contains the n‑th term. */
    while (true) {
        u128 len_count = count_len(L);
        if (cumulative + len_count >= (u128)n) break;
        cumulative += len_count;
        ++L;
    }

    /* 0‑based index within the chosen length */
    u128 k = (u128)n - cumulative - 1;

    unsigned int zeros_left = 4;
    unsigned int ones_left  = 2;
    seq_t val = 0;

    /* Unranking: build the number digit by digit. */
    for (unsigned int pos = 0; pos < L; ++pos) {
        for (int d = 0; d <= 3; ++d) {
            /* No leading zero. */
            if (pos == 0 && d == 0) continue;
            if (d == 0 && zeros_left == 0) continue;
            if (d == 1 && ones_left == 0) continue;

            unsigned int nz = zeros_left;
            unsigned int no = ones_left;
            if (d == 0) --nz;
            if (d == 1) --no;

            unsigned int rem = L - pos - 1;
            u128 ways = remaining_count(rem, nz, no);

            if (k >= ways) {
                k -= ways;                       /* skip this block */
                continue;
            }

            /* Digit d is part of the desired number. */
            val = val * (seq_t)4 + (seq_t)d;
            zeros_left = nz;
            ones_left  = no;
            break;                               /* proceed to next position */
        }
    }

    return val;
}
