/*
 * A098732  Consider the sequence {b(n), n >= 1} of digits of the integers: 0
 *          1 2 3 4 5 6 7 8 9 1 0 1 1 1 2 1 3 1 4 1 5 1 6 1 7 1 8 1 9 2 0...
 *          (A033307); a(n) = n - b(n).
 * https://oeis.org/A098732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n - d(n) where d(n) is the nth digit in the infinite
 *            string that consists of the decimal representations of the
 *            non‑negative integers concatenated in order
 *            (0,1,2,3,4,5,6,7,8,9,10,11,12,…)
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 89 terms through n = 990 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <limits.h>

static seq_t nth_digit(idx_t n) /* 1‑based index */
{
    idx_t rem = n;          /* remaining position to reach */
    idx_t k    = 1;         /* current digit length */
    idx_t pow10 = 1;        /* 10^(k-1) */

    /* Find the block (digit length) that contains the nth digit */
    while (1) {
        /* Count of numbers with k digits */
        idx_t count_k = (k == 1) ? 10ULL : 9ULL * pow10;

        /* Check for overflow in multiplication count_k * k */
        if (count_k == 0 || k > ULLONG_MAX / count_k) {
            /* Digits contributed by this block exceed max idx_t, so it must contain n */
            break;
        }

        idx_t digits_k = count_k * k;   /* digits contributed by this block */

        if (rem <= digits_k)
            break;                     /* n lies in this block */

        rem -= digits_k;  /* skip this block */

        /* Advance to next digit length */
        k++;
        if (pow10 > ULLONG_MAX / 10ULL) /* guard against overflow */
            pow10 = ULLONG_MAX;      /* will be used only if still needed */
        else
            pow10 *= 10ULL;
    }

    /* Position of the digit within this block */
    idx_t offset       = (rem - 1) / k;          /* zero‑based number offset */
    idx_t digit_index  = (rem - 1) % k;          /* zero‑based index inside the number */
    idx_t start_number = (k == 1) ? 0ULL : pow10; /* first number with k digits */

    idx_t number = start_number + offset;        /* the exact integer that contains the digit */

    /* Compute the digit at position digit_index (most significant = 0) */
    idx_t div = 1;
    for (idx_t i = 0; i < k - 1 - digit_index; ++i)
        div *= 10ULL;

    seq_t digit = (seq_t)((number / div) % 10ULL);
    return digit;
}

seq_t a(idx_t n)
{
    seq_t digit = nth_digit(n);
    return (seq_t)n - digit;
}
