/*
 * A083095  a(n) = A083094(n)/4.
 * https://oeis.org/A083095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 1024, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A083095
 *  Formula: a(n) = the n-th non‑negative integer k such that the base‑3
 *  representation of 2·k contains only the digits 0 and 1.
 *  (This is equivalent to a(n) = A074939(n‑1).)
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t k = 0;          /* candidate integer k */
    idx_t count = 0;      /* how many k's have satisfied the condition */

    for (;; ++k) {
        idx_t val = 2 * k;        /* we test 2·k only for digits */
        bool ok = true;

        /* handle the zero case (val == 0) – it satisfies the condition */
        while (val > 0) {
            if (val % 3 == 2) {   /* forbidden digit 2 in base‑3 */
                ok = false;
                break;
            }
            val /= 3;
        }

        if (ok) {
            ++count;
            if (count == n) {
                return (seq_t)k;   /* k itself is the required term */
            }
        }
    }
}
