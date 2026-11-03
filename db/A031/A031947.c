/*
 * A031947  Numbers in which 0,1,2,3,4,5 all occur in base 6.
 * https://oeis.org/A031947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A031947
 *  Formula: a(n) = the nth positive integer whose base‑6 representation
 *  contains all digits 0,1,2,3,4,5 at least once. */
#include <stdint.h>
#include <stdbool.h>

/* Helper: test whether a number in base 6 contains all digits 0–5. */
static bool has_all_digits_base6(seq_t v)
{
    unsigned mask = 0;
    if (v == 0) return false;              /* 0 does not have all digits */
    while (v) {
        unsigned d = (unsigned)(v % 6);    /* digit 0–5 */
        mask |= 1u << d;
        v /= 6;
    }
    return mask == 0x3Fu;                  /* 0b111111 = bits 0–5 set */
}

/*  Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* beyond offset – undefined */
    idx_t  count = 0;
    seq_t  value = 8345 - 1;              /* start just before first term */

    while (count < n) {
        ++value;
        if (has_all_digits_base6(value))
            ++count;
    }
    return value;
}
