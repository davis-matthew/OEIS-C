/*
 * A045021  Numbers whose base-4 representation contains exactly one 0 and
 *          three 1's.
 * https://oeis.org/A045021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5115, u32 n <= 9000, u64 n <= 9000,
 *            u128 n <= 9000, big any n
 * Validated: standard, pass, 60 terms through n = 8618 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑4 representation contains exactly one 0
   and three 1's.  The values are found by brute‑force enumeration of
   successive integers and testing the digit counts in base 4. */
#include <stdint.h>
#include <stdbool.h>

/* Return true iff x (>=1) written in base 4 contains exactly one digit
   0 and exactly three digits 1.  The test stops early if a count
   exceeds the required amount to keep the routine quick. */
static bool has_one_zero_three_ones(uint64_t x)
{
    unsigned cnt0 = 0, cnt1 = 0;
    while (x > 0) {
        unsigned d = x & 3U;          /* x % 4 */
        if (d == 0) {
            if (++cnt0 > 1) return false;
        } else if (d == 1) {
            if (++cnt1 > 3) return false;
        }
        x >>= 2;                       /* x /= 4 */
    }
    return (cnt0 == 1 && cnt1 == 3);
}

/* Return the n‑th term (n ≥ 1) of the sequence.  The function scans
   integers from 1 upward, keeps a running counter of numbers that
   satisfy the digit condition, and returns the value once the counter
   reaches n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for offset 1, but safe */
    idx_t target = n;
    idx_t count = 0;
    uint64_t v = 0;
    while (1) {
        ++v;
        if (has_one_zero_three_ones(v)) {
            ++count;
            if (count == target)
                return (seq_t)v;
        }
    }
}
