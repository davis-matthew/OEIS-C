/*
 * A126255  Number of distinct terms i^j for 2 <= i,j <= n.
 * https://oeis.org/A126255
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 262, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of distinct terms i^j for 2 <= i,j <= n
 *
 * The algorithm marks every integer i in [2,n] that is a perfect power.
 * For each remaining base a (not a perfect power) we determine the
 * maximum exponent Kmax with a^Kmax <= n.  For Kmax == 1 all
 * i = a gives distinct values a^j (j = 2..n) and this contributes
 * n-1 to the count.
 *
 * If Kmax > 1 we must account for the fact that i = a^k, k >= 1,
 * also appears.  The distinct values are a^e where e ranges over
 * all integers of the form e = k*j with
 *      1 <= k <= Kmax   and   2 <= j <= n.
 * For each such base a we create a small boolean array `seen`
 * of size Kmax*n + 1, mark all e in that range, and count the
 * number of marked entries.  The sum over all minimal bases
 * yields a(n).
 *
 * The implementation uses 128‑bit integers for intermediate
 * power calculations, and is otherwise compatible with C11
 * and C++17.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* offset 2 */

    /* Mark perfect powers up to n. */
    uint8_t *is_power = (uint8_t *)calloc((size_t)(n + 1), 1);
    if (!is_power) return 0;   /* allocation failure */

    for (idx_t base = 2; base <= n; ++base) {
        unsigned __int128 val = (unsigned __int128)base * base;
        while (val <= n) {
            is_power[(size_t)val] = 1;
            val *= base;
        }
    }

    seq_t total = 0;

    for (idx_t a_base = 2; a_base <= n; ++a_base) {
        if (is_power[(size_t)a_base]) continue;   /* not a minimal base */

        /* Determine Kmax: largest k with a_base^k <= n. */
        idx_t Kmax = 1;
        unsigned __int128 pw = a_base;
        while (pw <= n) {
            pw *= a_base;
            ++Kmax;
        }
        /* now Kmax is one greater than desired; subtract */
        --Kmax;

        if (Kmax == 1) {
            /* Only k = 1 contributes, giving e = j (2..n). */
            total += (seq_t)(n - 1);
            continue;
        }

        /* For Kmax > 1, generate all possible exponents e = k*j. */
        idx_t limit = Kmax * n;                  /* largest e */
        unsigned char *seen = (unsigned char *)calloc((size_t)(limit + 1), 1);
        if (!seen) continue;   /* allocation failure, skip this base */

        for (idx_t k = 1; k <= Kmax; ++k) {
            for (idx_t j = 2; j <= n; ++j) {
                idx_t e = k * j;
                seen[e] = 1;
            }
        }

        /* Count distinct e's. */
        seq_t count = 0;
        for (idx_t e = 2; e <= limit; ++e) {
            if (seen[e]) ++count;
        }

        total += count;
        free(seen);
    }

    free(is_power);
    return total;
}
