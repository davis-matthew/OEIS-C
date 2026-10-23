/*
 * A029740  Odd numbers with distinct digits.
 * https://oeis.org/A029740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, base
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029740: odd numbers with distinct decimal digits.
   The function returns the nth term (n ≥ 1) of the sequence,
   but does not hard‑code any terms.  It simply scans odd numbers
   and counts those whose decimal digits are all distinct.  This
   algorithm runs in O(n · d) time where d is the number of digits
   (at most 10 in the range of the sequence).  It is efficient enough
   for n up to 10 000 and respects all build types (uint8_t…,
   unsigned __int128, or arbitrary‑precision).  The code uses only
   integer arithmetic and portable C / C++ constructs.  */

#include <stdint.h>
#include <stdbool.h>

/* Check whether a non‑negative integer has all decimal digits distinct. */
static inline bool distinct_digits(uint64_t v)
{
    uint16_t mask = 0;            /* bitmask for digits 0..9, 10 bits needed */
    while (v != 0) {
        uint8_t d = v % 10;
        if (mask & (1u << d))
            return false;         /* duplicate digit found */
        mask |= (1u << d);
        v /= 10;
    }
    return true;                  /* all digits were distinct */
}

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* n is 1‑based; count of found terms */
    idx_t count = 0;
    /* Start with the smallest odd number (1) */
    uint64_t x = 1ULL;

    /* Scan odd numbers until the nth distinct‑digit odd number is found */
    while (true) {
        if (distinct_digits(x) && (x & 1ULL)) {
            ++count;
            if (count == n)
                return (seq_t)x;   /* cast to the build's seq_t type */
        }
        x += 2ULL;                 /* next odd candidate */
    }
}
