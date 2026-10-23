/*
 * A154873  Numbers k such that k^5 contains every digit exactly 3 times.
 * https://oeis.org/A154873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, full, nonn, fini
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for k such that k^5 has each decimal digit exactly three times */
#include <stdbool.h>
#include <stdint.h>

/* Local helper: tests whether k^5 contains every digit 0‑9 exactly three times. */
static bool is_valid(idx_t k)
{
    /* Compute k^5 as a 128‑bit integer (fits for k ≤ 10^6). */
    __int128 val = (__int128)k;
    val = val * val * val * val * val;   /* k^5 */

    unsigned freq[10] = {0};

    /* Count decimal digits of val. */
    while (val > 0) {
        int d = (int)(val % 10);
        ++freq[d];
        val /= 10;
    }

    /* Each digit must occur exactly three times. */
    for (int d = 0; d < 10; ++d)
        if (freq[d] != 3)
            return false;
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)    /* undefined for offset < 1 */
        return (seq_t)0;

    idx_t found = 0;
    const idx_t LIMIT = 1000000ULL;   /* 10^6 gives 30‑digit fifth powers */

    for (idx_t k = 1; k <= LIMIT; ++k) {
        if (is_valid(k)) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
    }
    /* n exceeds the number of terms found in the finite range. */
    return (seq_t)0;
}
