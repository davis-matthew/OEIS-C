/*
 * A039328  Numbers whose base-7 representation has the same nonzero number of
 *          5's and 6's.
 * https://oeis.org/A039328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039328: numbers whose base‑7 representation has the same
 * non‑zero number of 5's and 6's.  Implemented by brute‑force search. */
#include <stdbool.h>

/* Count whether v in base 7 contains an equal non‑zero number of 5's
 * and 6's.  The function returns true iff v has at least one 5 and one
 * 6 and their counts are equal. */
static bool equal_fives_sixes(seq_t v) {
    int cnt5 = 0, cnt6 = 0;
    seq_t t = v;
    while (t > 0) {
        seq_t d = t % 7;   /* digit in base 7 */
        if (d == 5) cnt5++;
        else if (d == 6) cnt6++;
        t = t / 7;
    }
    return (cnt5 == cnt6) && (cnt5 > 0);
}

/* Return the n-th term of A039328 (n ≥ 1). */
seq_t a(idx_t n) {
    seq_t x = 1;          /* candidate number */
    idx_t found = 0;      /* how many valid numbers have been found */

    while (1) {
        if (equal_fives_sixes(x)) {
            ++found;
            if (found == n)
                return x;
        }
        x = x + 1;        /* next candidate */
    }
}
