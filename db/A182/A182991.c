/*
 * A182991  Numbers with property that their divisors are odd, even, odd,
 *          even, and so on.
 * https://oeis.org/A182991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9733 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute by brute‑force search: a(n) is the n‑th integer whose
   divisors alternate in parity (odd, even, odd, even, ...). */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Return true if the divisors of x, listed in increasing order, alternate
   parity, starting with odd. */
static bool has_alternating_divisors(uint64_t x)
{
    /* divisors smaller than sqrt(x)   (ascending) */
    uint64_t small[256];
    /* divisors larger than sqrt(x)    (descending) */
    uint64_t large[256];
    size_t sz = 0, lg = 0;

    for (uint64_t i = 1; i <= x / i; ++i) {
        if (x % i == 0) {
            small[sz++] = i;
            uint64_t j = x / i;
            if (j != i)
                large[lg++] = j;
        }
    }

    bool prev_par = false;
    bool first = true;

    /* process the smaller divisors (ascending) */
    for (size_t k = 0; k < sz; ++k) {
        bool cur = (small[k] & 1);
        if (first) {
            prev_par = cur;
            first = false;
        } else {
            if (cur == prev_par)
                return false;
            prev_par = cur;
        }
    }

    /* process the larger divisors (descending) */
    for (int k = (int)lg - 1; k >= 0; --k) {
        bool cur = (large[k] & 1);
        if (cur == prev_par)
            return false;
        prev_par = cur;
    }

    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* sequence is defined from n = 1 */

    idx_t found = 0;
    for (uint64_t cand = 1; ; ++cand) {
        if (has_alternating_divisors(cand)) {
            ++found;
            if (found == n)
                return (seq_t)cand;
        }
    }
}
