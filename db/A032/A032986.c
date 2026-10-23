/*
 * A032986  Numbers with the property that all pairs of consecutive base-9
 *          digits differ by more than 2.
 * https://oeis.org/A032986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: numbers whose base‑9 digits differ by more than 2 */
/* The function checks candidates one by one; no caching or dynamic allocation is used. */

#include <stdbool.h>
#include <stdint.h>

/* Return true if every pair of consecutive base‑9 digits of x differ by > 2. */
static bool is_valid(seq_t x)
{
    if (x == 0)           /* 0 is not included in the sequence */
        return false;

    seq_t tmp = x;
    int prev_digit = -10;            /* sentinel for “no previous digit yet” */

    while (tmp > 0) {
        seq_t d_seq = tmp % 9;       /* current base‑9 digit (0 … 8) */
        int digit = (int)d_seq;      /* safe cast: the value is always < 9 */

        if (prev_digit >= 0) {
            int diff = prev_digit >= digit ? prev_digit - digit : digit - prev_digit;
            if (diff <= 2)          /* digits differ by ≤ 2 → invalid number */
                return false;
        }

        prev_digit = digit;
        tmp = tmp / 9;
    }
    return true;                     /* all pairs satisfied the condition */
}

/* Return the nth term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0)                /* sequence starts at a(1) */
        return (seq_t)0;

    seq_t candidate = 1;       /* smallest positive integer to test */
    idx_t found = 0;

    while (true) {
        if (is_valid(candidate)) {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;           /* next integer to test */
    }
}
