/*
 * A044728  Numbers n such that string 1,5 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) = numbers n such that decimal string "15" occurs
   in n but not in n+1.  Computed by a simple linear search. */

#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Test whether the decimal representation of x contains the
   two consecutive digits "15" (reading left‑to‑right). */
static inline bool has15(seq_t x) {
    int prev = -1;                        /* previous least‑significant digit */
    while (x > (seq_t)0) {
        int d = (int)(x % (seq_t)10);     /* current least‑significant digit */
        if (prev == 5 && d == 1)          /* we saw "...15" (reversed) */
            return true;
        prev = d;
        x = x / (seq_t)10;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.  It simply scans integers starting from 1
   until the n‑th qualifying number is reached.  The routine works
   for all supported seq_t types, including arbitrary‑precision
   classes, because it uses only the arithmetic operators that are
   guaranteed to exist for seq_t. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* safety: offset is 1 */

    seq_t num = 1;                        /* candidate number */
    idx_t count = 0;                      /* how many qualifiers found */

    for (;;) {
        if (has15(num) && !has15(num + (seq_t)1)) {
            ++count;
            if (count == n) return num;
        }
        num += 1;
    }
}
