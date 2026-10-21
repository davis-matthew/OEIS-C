/*
 * A073057  Start with the word 1234, repeatedly append the words obtained via
 *          the maps 1 -> 12, 2 -> 13, 3 -> 42, 4 -> 43.
 * https://oeis.org/A073057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth symbol (1..4) in the fixed point of the morphism 1 →
 *            12, 2 → 13, 3 → 42, 4 → 43, starting from 1234
 * Range:     u8 n <= 2900, u16 n <= 2900, u32 n <= 2900, u64 n <= 2900,
 *            u128 n <= 2900, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define MAX_CAP 100000   /* enough for n ≤ 10000 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* undefined for n < 1 */

    /* static cache of the generated word */
    static seq_t word[MAX_CAP];
    static idx_t len = 0;        /* current filled length */
    static int initialized = 0;  /* flag for first call */

    if (!initialized) {
        word[0] = 1; word[1] = 2; word[2] = 3; word[3] = 4;
        len = 4;
        initialized = 1;
    }

    while (len < n) {
        idx_t old_len = len;

        for (idx_t i = 0; i < old_len; ++i) {
            seq_t c = word[i];
            if (c == 1) { word[len++] = 1; word[len++] = 2; }
            else if (c == 2) { word[len++] = 1; word[len++] = 3; }
            else if (c == 3) { word[len++] = 4; word[len++] = 2; }
            else             { word[len++] = 4; word[len++] = 3; } /* c == 4 */
        }

        if (len > MAX_CAP) return 0;   /* safety for larger n */
    }

    return word[n - 1];   /* 1‑based index to 0‑based array */
}
