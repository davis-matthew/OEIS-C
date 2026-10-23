/*
 * A034306  Palindromes P such that Fibonacci iterations starting with (1, P)
 *          lead to a "nine digits anagram".
 * https://oeis.org/A034306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A034306 – palindromic starting numbers whose Fibonacci
   iterations hit a 9‑digit pandigital. (Computed by exhaustive search) */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_counts.h"   /* for digit_counts(seq_t, unsigned char[10]) */

/* Return true if x is a 9‑digit pandigital (digits 1..9 each once, no 0). */
static bool is_pandigital9(seq_t x)
{
    unsigned char cnt[10] = {0};
    digit_counts(x, cnt);
    if (cnt[0] != 0U) return false;
    for (int d = 1; d <= 9; ++d)
        if (cnt[d] != 1U) return false;
    return true;
}

/* Static cache of all palindromic starting values that eventually
   produce a 9‑digit pandigital in the Fibonacci iteration. */
static seq_t pal_list[80];
static size_t pal_count = 0;
static bool pal_initialized = false;

/* Generate all palindromic candidates up to 9 digits and test them. */
static void init_palindromes(void)
{
    if (pal_initialized)
        return;

    const seq_t PAN_MAX = (seq_t)987654321U;   /* largest 9‑digit pandigital */

    /* Iterate over lengths 1..9 */
    for (int len = 1; len <= 9; ++len) {
        int half = (len + 1) / 2;  /* number of digits in the left half */
        int start = 1;
        for (int i = 1; i < half; ++i) start *= 10;
        int end = start * 10 - 1;

        for (int left = start; left <= end; ++left) {
            /* Build the full palindrome. */
            seq_t pal = (seq_t)left;
            int temp = left;
            if (len & 1)          /* skip the middle digit for odd length */
                temp /= 10;
            while (temp > 0) {
                pal = pal * (seq_t)10U + (seq_t)(temp % 10);
                temp /= 10;
            }

            /* Run the Fibonacci sequence starting with (1, pal). */
            seq_t a_val = (seq_t)1U, b_val = pal;
            while (b_val <= PAN_MAX) {
                if (is_pandigital9(b_val)) {
                    pal_list[pal_count++] = pal;
                    break;
                }
                /* Prevent overflow: a_val + b_val must stay <= PAN_MAX. */
                if (a_val > PAN_MAX - b_val)
                    break;
                seq_t next = a_val + b_val;
                a_val = b_val;
                b_val = next;
            }
        }
    }
    pal_initialized = true;
}

/* Return the n‑th term of A034306 (1‑based indexing). */
seq_t a(idx_t n)
{
    init_palindromes();
    if (n == 0 || (size_t)n > pal_count)
        return (seq_t)0;
    return pal_list[(size_t)n - 1];
}
