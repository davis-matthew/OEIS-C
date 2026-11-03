/*
 * A044874  Numbers having, in base 3, (sum of even run lengths)=(sum of odd
 *          run lengths).
 * https://oeis.org/A044874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th positive integer whose base‑3 run‑lengths satisfy
   sum_of_even_runs == sum_of_odd_runs.  The function is evaluated
   independently for each call; it does not rely on any global state. */

#include <stdint.h>
#include <stdbool.h>

/* Return true if x has equal sums of even and odd run lengths in base 3. */
static bool base3_balanced(seq_t x)
{
    if (x == (seq_t)0) return false;

    seq_t sum_even = (seq_t)0;
    seq_t sum_odd  = (seq_t)0;
    seq_t t = x;

    while (t > (seq_t)0) {
        seq_t digit = t % (seq_t)3;
        t /= (seq_t)3;
        seq_t run = (seq_t)1;

        /* Count the length of this run of identical digits. */
        while (t > (seq_t)0) {
            seq_t next = t % (seq_t)3;
            if (next != digit) break;
            t /= (seq_t)3;
            run++;
        }

        /* Add the run length to the appropriate sum. */
        if ((run & (seq_t)1) == (seq_t)0)      /* even run length */
            sum_even += run;
        else
            sum_odd += run;
    }

    return sum_even == sum_odd;
}
/* seq_t and idx_t are defined by the build system. */

/* Return the n‑th term of OEIS sequence A044874 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) /* Undefined for n<1; return 0 as a safe fallback. */
        return (seq_t)0;

    idx_t found = (idx_t)0;
    seq_t candidate = (seq_t)1;

    while (found < n) {
        if (base3_balanced(candidate)) {
            found++;
            if (found == n)
                return candidate;
        }
        candidate++;
    }

    /* Control reaches here only if a wrap‑around occurs (impossible under
       normal circumstances).  Return the last tested candidate as a safety
       value. */
    return candidate;
}
