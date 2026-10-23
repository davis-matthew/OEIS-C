/*
 * A178843  Numbers in which the n-th digit is either a divisor or a nonzero
 *          multiple of n.
 * https://oeis.org/A178843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the nth positive integer whose decimal digits d_k
 * (counted from the left, 1‑based) satisfy d_k % k == 0 or k % d_k == 0.
 * This implementation computes a(n) by brute‑force search.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* true if x has the required property */
static bool valid_candidate(seq_t x)
{
    if (x == (seq_t)0) return false;

    /* extract digits in reverse order */
    uint8_t rev[128];          /* large enough for any reasonable seq_t */
    size_t len = 0;
    seq_t tmp = x;
    while (tmp != (seq_t)0)
    {
        rev[len++] = (uint8_t)(tmp % (seq_t)10);
        tmp = tmp / (seq_t)10;
    }

    /* test each digit from most significant to least (left to right) */
    for (size_t i = 0; i < len; ++i)
    {
        uint8_t d   = rev[len - 1 - i];
        size_t pos  = i + 1;    /* position is 1‑based from the left */

        if (d == 0)                     return false;
        if ((d % pos) != 0 && (pos % d) != 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, but harmless */

    size_t target = (size_t)n;
    size_t count  = 0;
    seq_t cand    = (seq_t)1;

    for (;;)
    {
        if (valid_candidate(cand))
        {
            ++count;
            if (count == target) return cand;
        }
        cand = cand + (seq_t)1;
    }
}
