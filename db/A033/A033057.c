/*
 * A033057  Numbers whose base-6 representation Sum_{i=0..m} d(i)*6^i has odd
 *          d(m-i) for all odd i.
 * https://oeis.org/A033057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑6 representation has the property that for all odd i,
   the digit d(m‑i) (where m is the index of the most significant digit)
   is odd.  The function enumerates these numbers in increasing order. */

#include <stdint.h>

static seq_t pow_seq_uint(seq_t base, uint64_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, but safe */

    /* 1. Find the length L of the base‑6 representation that contains the
       n‑th admissible number. */
    uint64_t L = 1;
    seq_t cumulative = 0;
    while (1) {
        /* Count admissible numbers of length L. */
        uint64_t m = L - 1;            /* index of most significant digit */
        uint64_t parity_m = m & 1;

        /* Number of positions (excluding leading) with the same parity as m */
        uint64_t same = 0, opp = 0;
        for (uint64_t j = 0; j < m; ++j) {
            if ((j & 1) == parity_m) ++same;
            else ++opp;
        }

        seq_t count = 5 * pow_seq_uint(6, same) * pow_seq_uint(3, opp);

        if (cumulative + count >= (seq_t)n) break;
        cumulative += count;
        ++L;
    }

    /* 2. Position of the desired number within the block of length L. */
    seq_t k = (seq_t)n - cumulative;   /* 1‑based */
    seq_t remaining = k - 1;           /* 0‑based */

    /* 3. Pre‑compute options, suffix products and 6‑powers. */
    seq_t opts[64];
    seq_t suffix[64];
    seq_t pow6[64];

    uint64_t m = L - 1;
    uint64_t parity_m = m & 1;

    /* Set options for each position. */
    for (uint64_t pos = 0; pos < L; ++pos) {
        if (pos == m) {
            opts[pos] = 5;        /* leading digit: 1..5 */
        } else if ((pos & 1) == parity_m) {
            opts[pos] = 6;        /* same parity as leading: unrestricted 0..5 */
        } else {
            opts[pos] = 3;        /* opposite parity: must be odd 1,3,5 */
        }
    }

    /* Suffix products: product of options for positions < pos */
    suffix[0] = 1;
    for (uint64_t pos = 1; pos < L; ++pos) {
        suffix[pos] = suffix[pos - 1] * opts[pos - 1];
    }

    /* Powers of 6 for positional values. */
    pow6[0] = 1;
    for (uint64_t pos = 1; pos < L; ++pos) {
        pow6[pos] = pow6[pos - 1] * (seq_t)6;
    }

    /* 4. Choose digits from most significant to least. */
    seq_t result = 0;
    for (int64_t pos = (int64_t)L - 1; pos >= 0; --pos) {
        seq_t idx_digit = remaining / suffix[pos];
        remaining -= idx_digit * suffix[pos];

        seq_t digit;
        if (pos == (int64_t)m) {                     /* leading digit */
            digit = (seq_t)1 + idx_digit;            /* 1 .. 5 */
        } else if ((pos & 1) != parity_m) {          /* constrained (must be odd) */
            digit = (seq_t)1 + (seq_t)2 * idx_digit; /* 1,3,5 */
        } else {                                     /* unrestricted */
            digit = idx_digit;
        }

        result += digit * pow6[pos];
    }

    return result;
}
