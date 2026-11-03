/*
 * A239706  Number of bases b for which the absolute value of the base-b
 *          alternate digital sum of n is b.
 * https://oeis.org/A239706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of bases b (1 ≤ b ≤ n) such that |altDigitSum_b(n)| = b
 *            (OEIS A239706). The computation is a direct brute‑force
 *            evaluation of the alternate digit sum for each base, which is
 *            O(n log n) operations and easily fast enough for n up to 10 000
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9694 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */

/* Helper: Computes |altDigitSum_b(n)|.
   For base 1 the unary expansion is used; otherwise the usual
   alternating digit sum is computed.  Only integer arithmetic is
   employed, so the routine works for all supplied seq_t types. */
static seq_t abs_alt_digit_sum(idx_t n, idx_t b)
{
    if (b == 1) {
        /* Unary base:  n = 11…1 (n ones).  altDigitSum_1(n) = n mod 2. */
        return (n & 1ULL) ? (seq_t)1 : (seq_t)0;
    }

    seq_t sum_even = 0;   /* sum of digits at even positions (0‑based) */
    seq_t sum_odd  = 0;   /* sum of digits at odd  positions (0‑based) */
    seq_t N = (seq_t)n;
    seq_t base = (seq_t)b;
    int pos = 0;          /* position of the least significant digit   */

    while (N > 0) {
        seq_t digit = N % base;
        if (pos & 1)
            sum_even += digit;
        else
            sum_odd += digit;
        N /= base;
        ++pos;
    }

    /* Return the absolute difference.  The comparison with the larger
       sum also works when the sums are equal (difference zero). */
    return (sum_odd > sum_even) ? (sum_odd - sum_even) : (sum_even - sum_odd);
}

/* Forward declaration for the helper that gives the absolute
   alternate digit sum in base b. */
static seq_t abs_alt_digit_sum(idx_t n, idx_t b);

/* Public sequence function */
seq_t a(idx_t n)
{
    /* The OEIS sequence is defined for n ≥ 1; for safety return 0 for n = 0. */
    if (n == 0)
        return 0;

    seq_t count = 0;

    for (idx_t b = 1; b <= n; ++b) {
        seq_t abs_sum = abs_alt_digit_sum(n, b);
        if (abs_sum == (seq_t)b)
            ++count;
    }
    return count;
}

/* ------------------------------------------------------------------ */
