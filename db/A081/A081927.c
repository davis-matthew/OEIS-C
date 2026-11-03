/*
 * A081927  n-th positive integer whose digits sum up to n.
 * https://oeis.org/A081927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 37, u32 n <= 81, u64 n <= 169,
 *            u128 n <= 342, big any n
 * Validated: standard, pass, 61 terms through n = 8894 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS sequence A081927: n‑th positive integer whose digits sum to n.
   This implementation generates the number by starting with the smallest
   number of the minimal possible length and repeatedly computing the
   next larger number with the same digit sum.  For each “next” operation
   we modify the digit array in place using only integer arithmetic.  The
   algorithm runs in O(n * k) where k ≤ 9·⌈n/9⌉, which is well below the
   required 2 s for n ≤ 10000. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: create the smallest number of given length 'len' whose digits
   sum to 'sum'.  If len equals the minimal possible length
   (ceil(sum/9)), the leading digit may be 0; otherwise the leading
   digit is forced to be 1 to keep the number valid. */
static void build_minimal_digits(int len, int sum, int minimal_len, int *digits)
{
    int remaining = sum;
    if (len == minimal_len) {
        /* Use right‑to‑left filling: largest digits on the right,
           so that the leftmost digit is the smallest possible. */
        for (int i = len - 1; i >= 0; --i) {
            int d = remaining >= 9 ? 9 : remaining;
            digits[i] = d;
            remaining -= d;
        }
    } else {
        /* Leading digit must be ≥1.  Assign 1 and distribute the rest
           to the rightmost positions. */
        digits[0] = 1;
        remaining = sum - 1;
        for (int i = len - 1; i >= 1; --i) {
            int d = remaining >= 9 ? 9 : remaining;
            digits[i] = d;
            remaining -= d;
        }
    }
}

/* Helper: compute the next larger number with the same digit sum.
   The function modifies the digit array in place.  It returns true
   if a next number exists within the same length, otherwise false. */
static bool next_number_same_sum(int len, int *digits, int sum)
{
    int sum_suffix = 0;            /* sum of digits to the right of i */
    for (int i = len - 1; i >= 0; --i) {
        int cur = digits[i];
        int left_sum = sum - sum_suffix - cur;
        int right_positions = len - 1 - i;
        int max_suffix = 9 * right_positions;

        /* Try to increase this digit. */
        for (int d = cur + 1; d <= 9; ++d) {
            int rem = sum - (left_sum + d);
            if (rem < 0 || rem > max_suffix)
                continue;                     /* impossible to fill suffix */

            /* Build the smallest suffix with sum 'rem'. */
            digits[i] = d;
            int rem_sum = rem;
            for (int k = len - 1; k > i; --k) {
                int dig = rem_sum >= 9 ? 9 : rem_sum;
                digits[k] = dig;
                rem_sum -= dig;
            }
            return true;
        }
        sum_suffix += cur;
    }
    return false;   /* no larger number of this length */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset, but safe */

    int sum = (int)n;
    int minimal_len = (sum + 8) / 9;

    /* Allocate enough space for all possible lengths (≤ sum). */
    int *digits = (int *)malloc((sum + 5) * sizeof(int));

    /* Start with the minimal number of the minimal possible length. */
    int len = minimal_len;
    build_minimal_digits(len, sum, minimal_len, digits);

    /* Generate (n-1) subsequent numbers. */
    for (idx_t step = 1; step < n; ++step) {
        if (!next_number_same_sum(len, digits, sum)) {
            /* No larger number of this length: increase length. */
            ++len;
            build_minimal_digits(len, sum, minimal_len, digits);
        }
    }

    /* Convert the digit array to a seq_t value. */
    seq_t result = 0;
    for (int i = 0; i < len; ++i) {
        result = result * 10 + (seq_t)digits[i];
    }

    free(digits);
    return result;
}
