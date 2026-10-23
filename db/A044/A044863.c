/*
 * A044863  Positive integers having the same number of base-7 runs of odd
 *          length as even.
 * https://oeis.org/A044863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th positive integer whose base‑7 representation
 *            has an equal number of runs of odd length and runs of even
 *            length. The function finds the value by brute‑force search
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Count runs in the base‑7 representation of x; return true if the
   number of odd‑length runs equals the number of even‑length runs. */
static bool runs_equal(seq_t x)
{
    int prev = -1;          /* previous digit, -1 means none yet */
    int run_len = 0;        /* length of current run */
    seq_t odd = 0;          /* count of odd‑length runs */
    seq_t even = 0;         /* count of even‑length runs */

    if (x == 0) return false;  /* only positive integers are considered */

    while (x > 0) {
        int d = (int)(x % (seq_t)7);  /* next digit (least significant) */
        x /= (seq_t)7;

        if (prev == -1) {            /* first digit */
            prev = d;
            run_len = 1;
        } else if (d == prev) {      /* continue current run */
            run_len++;
        } else {                     /* run ended */
            if ((run_len & 1) == 0) even++;
            else odd++;
            prev = d;
            run_len = 1;
        }
    }

    /* account for the last run */
    if (run_len > 0) {
        if ((run_len & 1) == 0) even++;
        else odd++;
    }

    return odd == even;
}

seq_t a(idx_t n)
{
    seq_t candidate = (seq_t)1;
    idx_t matched = (idx_t)0;

    while (1) {
        if (runs_equal(candidate)) {
            matched++;
            if (matched == n)
                return candidate;
        }
        candidate++;
    }
}
