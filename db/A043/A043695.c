/*
 * A043695  Numbers whose base-6 representation has an even number of runs.
 * https://oeis.org/A043695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer whose base‑6 representation has an even
 *            number of runs (computed by scanning)
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether v (in base 6) contains an even number of runs. */
static bool has_even_runs(seq_t v)
{
    if (v < 6)        /* numbers < 6 have < 2 digits and are not in the sequence */
        return false;

    seq_t prev = v % 6;       /* least significant digit */
    v /= 6;
    unsigned int runs = 1;    /* the first run (the last digit) */

    while (v > 0) {
        seq_t d = v % 6;
        v /= 6;
        if (d != prev) {      /* a new run starts */
            prev = d;
            runs += 1;
        }
    }
    return (runs & 1) == 0;   /* true iff number of runs is even */
}

/* Return the n-th term (n >= 1).  The function scans numbers upward
 * from 6 until the required count has been reached.  The time
 * complexity is O(n * log6(n)), which is well below the 2.0‑second
 * limit for n up to about 10,000. */
seq_t a(idx_t n)
{
    if (n == 0)          /* behaviour below the offset is undefined; return 0 */
        return 0;

    seq_t candidate = 6;     /* first term in the sequence */
    idx_t counted = 0;

    while (1) {
        if (has_even_runs(candidate)) {
            counted++;
            if (counted == n)
                return candidate;
        }
        candidate++;          /* advance to the next integer */
    }
}
