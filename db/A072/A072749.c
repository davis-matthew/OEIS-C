/*
 * A072749  Count of factors of 5 in the first n squarefree numbers.
 * https://oeis.org/A072749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of the first n square‑free numbers that are *
 *            divisible by 5. The function enumerates integers in increasing
 *            order, * counts the first n square‑free ones and tallies how
 *            many of them are * multiples of 5. Square‑freeness is checked by
 *            testing whether any * prime square divides the i...
 * Range:     u8 n <= 1525, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>


/* local helper: test whether x is square‑free */
static bool is_squarefree_uint64(uint64_t x)
{
    for (uint64_t p = 2; p * p <= x; ++p) {
        uint64_t sq = p * p;
        if (x % sq == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t count = (seq_t)0;   /* number of 5‑divisible square‑free numbers seen */
    idx_t found = 0;          /* how many square‑free numbers encountered */
    uint64_t num = 1;         /* current integer being tested */

    while (found < n) {
        if (is_squarefree_uint64(num)) {
            ++found;
            if (num % 5 == 0)
                ++count;
        }
        ++num;
    }
    return count;
}
