/*
 * A124673  Numbers with distinct prime digits.
 * https://oeis.org/A124673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with distinct prime digits (OEIS A124673). */
/* a(n) = the nth smallest integer composed of distinct digits from {2,3,5,7}. */

#include <stddef.h>   /* size_t, NULL */
#include <stdbool.h>  /* bool */
#include <stdint.h>   /* uint8_t, uint16_t, uint32_t, uint64_t, etc. */
#include <stdlib.h>   /* qsort */

/* Static storage for all values that fit in seq_t.  The maximum
   possible number with these digits is 7532, so at most 64 numbers. */
static seq_t seq_arr[64];
static size_t seq_count = 0;
static bool initialized = false;

/* Comparison function for qsort.  Works for any unsigned integer type
   or for an arbitrary‑precision class that overloads the comparison
   operators. */
static int seq_cmp(const void *pa, const void *pb)
{
    const seq_t *a = (const seq_t *)pa;
    const seq_t *b = (const seq_t *)pb;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Recursive generator that builds numbers digit by digit.
   cur  - current number being built (as uint64_t to avoid overflow)
   used - bitmask of digits that have already been used
   target - remaining number of digits to place. */
static void gen_numbers(uint64_t cur, unsigned used, int target)
{
    if (target == 0) {
        /* Convert to seq_t and check for overflow. */
        seq_t val = (seq_t)cur;
        if ((uint64_t)val == cur) {
            seq_arr[seq_count++] = val;
        }
        return;
    }

    const uint64_t digits[4] = {2, 3, 5, 7};
    for (int i = 0; i < 4; ++i) {
        if (!(used & (1U << i))) {
            gen_numbers(cur * 10 + digits[i], used | (1U << i), target - 1);
        }
    }
}

/* Initialise the sequence array once. */
static void init_sequence(void)
{
    seq_count = 0;
    /* Lengths 1 to 4. */
    for (int len = 1; len <= 4; ++len) {
        gen_numbers(0, 0, len);
    }
    /* Sort numerically. */
    qsort(seq_arr, seq_count, sizeof(seq_t), seq_cmp);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_sequence();
    }
    /* n is guaranteed to be >= 1.  For n beyond the defined terms we
       return 0.  The behaviour for n > seq_count is undefined by OEIS,
       but returning 0 is a safe fallback. */
    if (n < 1 || (size_t)n > seq_count) {
        return 0;
    }
    return seq_arr[(size_t)n - 1];
}
