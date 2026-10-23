/*
 * A188201  The least base-n Keith number >= n.
 * https://oeis.org/A188201
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least integer m ≥ n such that m, when written in base n,
 *            has its digits form a Keith sequence that reaches m. (A base‑n
 *            Keith number is defined by the usual recursive definition
 *            starting from the base‑n digits.)
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n is ≥ 2 by definition of the sequence. */
    seq_t base = (seq_t)n;           /* convert once for speed */
    for (seq_t m = base; ; ++m) {
        /* base‑n representation of m: at least two digits because m ≥ n. */
        seq_t d1 = m / base;        /* most significant digit (≥1) */
        seq_t d2 = m % base;        /* least significant digit (may be 0) */

        /* generate Keith sequence from the two digits */
        seq_t prev = d1;
        seq_t curr = d2;
        while (true) {
            seq_t next = prev + curr;
            if (next == m) return m;        /* found the Keith number */
            if (next > m)  break;           /* cannot reach m with larger terms */
            prev = curr;
            curr = next;
        }
        /* otherwise continue with the next candidate m */
    }
}
