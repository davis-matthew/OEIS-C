/*
 * A236689  The sum of two neighboring digits is a palindrome; a(n) is the
 *          smallest possible nonnegative integer not occurring earlier.
 * https://oeis.org/A236689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   append_candidate, digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = smallest non‑negative integer not used before that
 * can be appended to the already grown prefix while keeping the
 * property that for every pair of neighboring decimal digits the sum
 * is < 10 or equal to 11.
 *
 * The implementation follows the greedy definition: at each step
 * search the integers from 0 upward and select the first unused one
 * that satisfies the adjacency condition.
 *
 * The sequence is memoised in static arrays so calls may appear in any
 * order.  All operations are performed with the provided helper
 * functions and basic C/C++ operators; no additional libraries are
 * required.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/append_candidate.h"
#include "../utils/digit_len.h"

/* Global storage of the sequence */
static seq_t *seq_arr   = NULL; /* sequence values */
static size_t seq_len   = 0;    /* current length */
static size_t seq_cap   = 0;    /* capacity */

/* Bitmap to record used numbers (assumes values fit in uint64_t) */
static uint64_t *used_bits   = NULL;
static size_t used_bits_cap  = 0; /* number of 64‑bit words */

/* Last digit of the current concatenated string (most recently used number) */
static int prev_last_digit = -1; /* -1 means no previous digit */

/* Ensure that the bitmap can index at least 'v' (inclusive) */
static void ensure_used_capacity(seq_t v)
{
    uint64_t val = (uint64_t)v;          /* candidate fits in 64 bits */
    size_t need_words = (size_t)(val >> 6) + 1;
    if (need_words <= used_bits_cap) return;

    size_t new_cap = used_bits_cap ? used_bits_cap * 2 : 16;
    if (need_words > new_cap) new_cap = need_words;

    uint64_t *tmp = (uint64_t*)realloc(used_bits, new_cap * sizeof(uint64_t));
    if (!tmp) abort();              /* allocation failure */

    for (size_t i = used_bits_cap; i < new_cap; ++i) tmp[i] = 0;
    used_bits = tmp;
    used_bits_cap = new_cap;
}

/* Test whether a number is valid to append with the current prefix
   (prev_last_digit may be -1 for the very first number). */
static bool is_valid_candidate(seq_t num, int prev_last)
{
    size_t len = digit_len(num);
    if (!len) return false; /* should not happen for non‑negative integers */

    /* Allocate buffer for the digits in least‑significant first order */
    uint8_t *d = (uint8_t*)malloc(len);
    if (!d) abort();

    seq_t tmp = num;
    for (size_t i = 0; i < len; ++i) {
        d[i] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    /* Internal sums: works irrespective of digit order because addition
       is commutative. */
    for (size_t i = 0; i + 1 < len; ++i) {
        int sum = d[i] + d[i + 1];
        if (sum > 9 && sum != 11) {
            free(d);
            return false;
        }
    }

    /* Cross‑boundary sum with the previous last digit, if any */
    if (prev_last >= 0) {
        int sum = prev_last + d[len - 1]; /* most significant digit */
        if (sum > 9 && sum != 11) {
            free(d);
            return false;
        }
    }

    free(d);
    return true;
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    /* Grow the sequence until we have the desired index */
    while (seq_len <= (size_t)n) {

        /* Search from 0 upward for the first unused candidate that
           satisfies the adjacency condition with the current prefix. */
        seq_t cand = 0;
        for (;;) {
            /* Ensure the bitmap has room for this candidate */
            ensure_used_capacity(cand);
            uint64_t idx = (uint64_t)cand;
            size_t word = (size_t)(idx >> 6);
            uint64_t mask = 1ULL << (idx & 63);

            /* Skip already used numbers */
            if (!(used_bits[word] & mask)) {
                /* Test the adjacency rule */
                if (is_valid_candidate(cand, prev_last_digit)) {
                    /* Mark as used */
                    used_bits[word] |= mask;

                    /* Append to the sequence */
                    append_candidate(&seq_arr, &seq_len, &seq_cap, cand);

                    /* Update the last digit for future cross‑boundary checks */
                    int last_digit = (int)(cand % 10);
                    prev_last_digit = last_digit;

                    break; /* move on to generate the next term */
                }
            }
            ++cand;
        }
    }

    return seq_arr[(size_t)n];
}
