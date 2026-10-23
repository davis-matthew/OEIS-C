/*
 * A043888  Numbers k such that 0 and 3 occur juxtaposed in the base-5
 *          representation of k but not of k+1.
 * https://oeis.org/A043888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers k such that 
   (a) the base‑5 representation of k contains adjacent digits 0 and 3 in
   either order (i.e., the substring "03" or "30"), and
   (b) the base‑5 representation of k+1 does NOT contain such a pair. */

#include <stdbool.h>

static bool has_adjacent_0_3(seq_t n) {
    int prev = -1;               /* previous digit (next higher place) */
    while (n) {
        int cur = (int)(n % 5);  /* current least significant digit */
        if ((cur == 0 && prev == 3) || (cur == 3 && prev == 0))
            return true;                 /* found 03 or 30 */
        prev = cur;
        n /= 5;
    }
    return false;                /* no adjacent 0‑3 pair */
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                      /* offset is 1, below that undefined */
    seq_t k = 0;
    idx_t found = 0;
    while (1) {
        if (has_adjacent_0_3(k) && !has_adjacent_0_3(k + 1)) {
            ++found;
            if (found == n)
                return k;
        }
        ++k;
    }
}
