/*
 * A099054  Arshon's sequence: start from 1 and replace the letters in odd
 *          positions using 1 -> 123, 2 -> 231, 3 -> 312 and the letters in
 *          even positions using 1 -> 321, 2-> 132, 3 -> 213.
 * https://oeis.org/A099054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A099054 (Arshon's sequence)
   Computed by iterating a 3‑ary substitution where
   odd positions use the mapping 1→123, 2→231, 3→312
   (in 0‑based index even positions, which are odd in 1‑based
   counting), and even positions use the reverse mapping
   1→321, 2→132, 3→213.  The term is the (n‑th) symbol in
   the infinite limit word, indexed from 0. */
#include <stdbool.h>

/* Return the n-th term of A099054. */
seq_t a(idx_t n) {
    /* The first term is 1. */
    if (n == 0) return 1;

    /* Find the highest power of 3 not exceeding n. */
    idx_t d = 1;
    while (d <= n / 3) d *= 3;

    seq_t L = 0;          /* Current symbol (0‑based: 0→1, 1→2, 2→3). */
    bool even_flag = true;/* Parent index is even (0‑based), i.e., position
                            is odd in 1‑based indexing -> use mapping 1. */

    while (d > 0) {
        idx_t r = n / d;  /* Current base‑3 digit (0, 1, 2). */
        n %= d;
        d /= 3;

        if (even_flag) {
            /* Mapping for odd 1‑based positions: (L + r) mod 3. */
            L = (L + (seq_t)r) % 3;
            /* Update flag: new parent index is even when r is even. */
            even_flag = ((r & 1) == 0);
        } else {
            /* Mapping for even 1‑based positions: (L + 2 – r) mod 3. */
            L = (L + (seq_t)(2 - r)) % 3;
            /* Update flag: new parent index is even when r is odd. */
            even_flag = ((r & 1) == 1);
        }
    }

    return (seq_t)(L + 1); /* Convert back to 1‑based symbol. */
}
