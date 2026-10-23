/*
 * A043496  Numbers having four 1's in base 10.
 * https://oeis.org/A043496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers having four 1's in base 10 (search) */
#include <stdbool.h>

/* Returns true iff x has exactly four digits equal to 1 in its decimal representation. */
static inline bool has_four_ones(idx_t x)
{
    int cnt = 0;
    while (x > 0) {
        if (x % 10 == 1) {
            ++cnt;
            if (cnt > 4)
                return false;
        }
        x /= 10;
    }
    return cnt == 4;
}

/* External linkage: a(n) = nth positive integer whose decimal notation contains exactly four '1' digits. */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for offset 1; return 0 as a safe default */
        return (seq_t)0;
    
    idx_t found = 0;
    idx_t value = 1111;         /* first term of the sequence */

    for ( ; ; ++value) {
        if (has_four_ones(value)) {
            ++found;
            if (found == n)
                return (seq_t)value;
        }
    }
}
