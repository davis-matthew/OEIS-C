/*
 * A033005  Every run of digits of n in base 7 has length 2.
 * https://oeis.org/A033005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 8 * (base‑49 number with digits 0..6, leading digit
 *            non‑zero)
 * Range:     u8 n <= 6, u16 n <= 150, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the sequence of integers whose base‑7 representation consists
   of runs of equal digits, each run having length exactly two.  The
   mapping to base‑49 is: each run of two identical base‑7 digits is
   represented by one base‑49 digit.  Hence for a run count k the
   corresponding number is 8 times the base‑49 number composed of a
   k‑digit sequence d₁d₂…d_k with d₁ ∈ {1,…,6}, d_i ≠ d_{i‑1},
   d_i ∈ {0,…,6} for i>1.  The terms are ordered by increasing
   decimal value, which coincides with lexicographic order of the
   digit sequences.  The following implementation directly
   decodes the index n into such a digit sequence without any
   recursion or caching. */

#include <stdint.h>

static const idx_t pow6[32] = {
    1ULL,
    6ULL,
    36ULL,
    216ULL,
    1296ULL,
    7776ULL,
    46656ULL,
    279936ULL,
    1679616ULL,
    10077696ULL,
    60466176ULL,
    362797056ULL,
    2176782336ULL,
    13060694016ULL,
    78364164096ULL,
    470184984576ULL,
    2821109907456ULL,
    16926659444736ULL,
    101559956668416ULL,
    609359740010496ULL,
    3656158440062976ULL,
    21936950640377856ULL,
    131621703842267136ULL,
    789730221493602816ULL,
    4738381338321616896ULL,
    28430228029929701376ULL,
    170581368179578208256ULL,
    1023494209077469253632ULL,
    6148964854464815521792ULL,
    36893789126768893130752ULL,
    221362734760613358984832ULL
};

seq_t a(idx_t n)
{
    /* n must be >= 1; for n == 0 return 0 (not used) */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Find length k such that cumulative count up to k-1 < n <= cumulative up to k */
    idx_t cumulative = 0;
    int k = 1;
    for (; ; ++k) {
        if (n <= cumulative + pow6[k]) {
            break;
        }
        cumulative += pow6[k];
    }

    /* Offset in the block of length k (0‑based) */
    idx_t offset = n - cumulative - 1;   /* 0 <= offset < pow6[k] */

    /* Decode first digit */
    idx_t first_idx = offset / pow6[k - 1];   /* 0..5 */
    idx_t prev_digit = first_idx + 1;         /* 1..6 */

    /* Prepare value in base-49 */
    seq_t value = (seq_t)prev_digit;          /* most significant digit */

    /* Process remaining digits (k-1 of them) */
    idx_t remaining = offset % pow6[k - 1];
    for (int pos = 2; pos <= k; ++pos) {
        idx_t base = pow6[k - pos];
        idx_t digit_idx = remaining / base;   /* 0..5 */
        remaining %= base;

        /* Map index to digit excluding the previous one */
        idx_t digit = (digit_idx >= prev_digit) ? digit_idx + 1 : digit_idx;
        prev_digit = digit;

        value = value * (seq_t)49 + (seq_t)digit;
    }

    /* Multiply by 8 to obtain the final term */
    return value * (seq_t)8;
}
