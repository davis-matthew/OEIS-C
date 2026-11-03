/*
 * A076164  Numbers k such that sum of squares of even digits of k equals sum
 *          of squares of odd digits.
 * https://oeis.org/A076164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth nonnegative integer k such that the sum of squares of
 *            even digits of k equals the sum of squares of odd digits. The
 *            function is implemented by a straightforward brute‑force search,
 *            which is fast enough for the required n (on the order of 10⁴)
 * Range:     u8 n <= 1, u16 n <= 71, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 50 terms through n = 8740 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool balanced_uint64(unsigned long long x)
{
    unsigned even = 0;
    unsigned odd  = 0;

    if (x == 0ULL)              /* 0 has digit 0 → sum_even == sum_odd == 0 */
        return true;

    while (x) {
        unsigned d   = (unsigned)(x % 10ULL);
        unsigned sq  = d * d;
        if ((d & 1U) == 0U)     /* even digit */
            even += sq;
        else                    /* odd digit */
            odd  += sq;
        x /= 10ULL;
    }
    return even == odd;
}

/* External linkage as required */
seq_t a(idx_t n)
{
    idx_t  count = 0;          /* how many valid numbers found so far */
    unsigned long long k = 0;  /* current integer being tested */

    while (true) {
        if (balanced_uint64(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
