/*
 * A230453  Number of terms in the "comma sum" sequence (cf. A230288) starting
 *          with n, or 0 if it is infinite.
 * https://oeis.org/A230453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of terms in the “comma sum” sequence that starts *
 *            with the integer n, or 0 if the sequence never terminates. * * A
 *            “comma sum” sequence is defined by: * a(k+1) - a(k) = rightmost
 *            digit of a(k) + leftmost digit of a(k+1), * with the next term
 *            chosen as the smallest integer that sati...
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Compute the next term of a comma‑sum sequence.
 * Returns true and stores the next term in *next when it exists,
 * or returns false if no next term can be found. */
static bool next_term(seq_t cur, seq_t *next)
{
    seq_t r = cur % 10;                 /* rightmost decimal digit of cur */
    for (unsigned int L = 1; L <= 9; ++L) {
        seq_t cand = cur + r + (seq_t)L; /* candidate next term */

        /* Determine leftmost decimal digit of candidate. */
        seq_t left = cand;
        while (left >= 10)
            left /= 10;

        if (left == (seq_t)L) {
            *next = cand;
            return true;
        }
    }
    return false;                       /* no valid next term */
}

/* Main routine requested by the test harness. */
seq_t a(idx_t n)
{
    seq_t cur   = (seq_t)n;   /* starting value of the sequence   */
    seq_t count = 1;          /* a(0) is counted as the first term */

    /* Tortoise (slow) and hare (fast) pointers for cycle detection. */
    seq_t tortoise = cur;
    seq_t hare      = cur;
    bool hare_valid = true;   /* whether hare has not yet hit the end */

    for (;;) {
        seq_t next_t;         /* next value for tortoise */

        /* Advance tortoise by one step.  If it cannot move, the
         * sequence terminates and we have counted the full length. */
        if (!next_term(tortoise, &next_t))
            break;
        tortoise = next_t;
        ++count;

        /* Advance hare by two steps if it has not yet terminated. */
        if (hare_valid) {
            seq_t next_h1, next_h2;
            if (!next_term(hare, &next_h1)) {
                hare_valid = false;
            } else if (!next_term(next_h1, &next_h2)) {
                hare_valid = false;
            } else {
                hare = next_h2;
            }
        }

        /* If both pointers coincide, the sequence is an infinite cycle. */
        if (hare_valid && tortoise == hare)
            return (seq_t)0;
    }

    /* The sequence has terminated after 'count' terms. */
    return count;
}
