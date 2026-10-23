/*
 * A153159  a(n) = A007916(n)^5.
 * https://oeis.org/A153159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p(n))^5 * where p(n) is the n‑th positive integer that
 *            is NOT a perfect power * (i.e. not of the form k^e with integers
 *            k>1, e>1). The sequence * begins with
 *            2,3,5,6,7,10,11,12,13,14,15,17,18,19,20,21,22,23,24,26,28,29,30,31,...
 *            * and a(n) is the 5th power of p(n). No hard coded tables ar...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 72, u64 n <= 7026,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether m (m >= 1) is a perfect power: m = a^e with a>1, e>1. */
static bool is_perfect_power_u64(uint64_t m)
{
    if (m < 4)            /* 1,2,3 are not perfect powers */
        return false;

    /* Trial exponents e from 2 upwards.  Stop when 2^e > m. */
    for (int e = 2; e <= 63; e++) {
        /* 2^e in 128‑bit to avoid overflow in shift */
        if (((unsigned __int128)1 << e) > m)
            break;

        /* Binary search for a such that a^e = m. */
        uint64_t low = 2, high = m;
        while (low <= high) {
            uint64_t mid = low + (high - low) / 2;

            /* Compute mid^e with early stop if > m. */
            unsigned __int128 power = 1;
            for (int k = 0; k < e; k++) {
                power *= (unsigned __int128)mid;
                if (power > m)
                    break;
            }

            if (power == m)
                return true;
            if (power < m)
                low = mid + 1;
            else
                high = mid - 1;
        }
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)  /* behaviour for index 0 is unused, return 0 safely */
        return 0;

    idx_t seen = 0;           /* count of non‑perfect‑power numbers found */
    uint64_t m = 2;           /* candidate numbers start at 2 */

    while (1) {
        if (!is_perfect_power_u64(m)) {
            seen++;
            if (seen == n) {
                seq_t base = (seq_t)m;
                seq_t res  = 1;
                for (int k = 0; k < 5; ++k)
                    res *= base;
                return res;
            }
        }
        m++;
    }
}
