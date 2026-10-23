/*
 * A125290  Numbers with at least two distinct digits in decimal
 *          representation, none of which is 0.
 * https://oeis.org/A125290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 192, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A125290
 *  a(n) = the nth positive integer whose decimal representation
 *  contains at least two distinct non‑zero digits.
 *  (Thus 12, 13, 14, …, 21, 23, etc.)
 *
 *  The function implements the definition by iterating through
 *  successive integers and counting those that satisfy the
 *  digit condition.  No caching or external helpers are used,
 *  so the implementation works for all supported seq_t types
 *  (including arbitrary‑precision integer classes) and for all
 *  permissible idx_t values. */
 
#include <stdbool.h>
 
/* Test whether x has at least two distinct digits, none of which is 0. */
static bool has_valid_digits(seq_t x)
{
    if (x < (seq_t)10)           /* single‑digit numbers fail */
        return false;
 
    const seq_t ten = (seq_t)10;
    const seq_t zero = (seq_t)0;
    unsigned mask = 0;           /* bits for digits 1–9 */
    while (x > zero)
    {
        seq_t d = x % ten;        /* current decimal digit */
        if (d == zero)           /* digit 0 is disallowed */
            return false;
        unsigned di = (unsigned)d - 1;   /* map 1–9 -> 0–8 */
        mask |= 1u << di;
        x /= ten;
    }
    /* At least two distinct digits iff mask has at least two bits. */
    return (mask & (mask - 1)) != 0;
}
 
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n < 1, return 0 */
 
    seq_t count = 0;                    /* number of valid terms seen */
    seq_t candidate = (seq_t)12;         /* smallest integer with two digits */
 
    while (count < n)
    {
        if (has_valid_digits(candidate))
        {
            count++;
            if (count == n)
                return candidate;
        }
        candidate++;                    /* advance to the next integer */
    }
    /* The loop always returns, but keep a return to satisfy the compiler */
    return candidate;
}
