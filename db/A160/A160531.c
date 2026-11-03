/*
 * A160531  Positive integers that contain both odd- and even-length runs of
 *          0's and 1's in their binary representation.
 * https://oeis.org/A160531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth positive integer whose binary representation *
 *            contains at least one run of 0's or 1's of odd length and at
 *            least one * run of 0's or 1's of even length (runs are maximal
 *            contiguous * substrings of equal bits). This function finds the
 *            term by brute‑force * search; it works for any...
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true iff x has both an odd‑length run and an even‑length run
 * of 0's or 1's in its binary representation. */
static bool has_both_runs(seq_t x)
{
    if (x == 0) return false;          /* skip the empty representation */

    bool last_bit = false;
    bool first_run = true;
    int run_len = 0;
    bool has_odd  = false;
    bool has_even = false;

    /* Scan bits from least significant to most significant. */
    seq_t temp = x;
    while (temp != 0) {
        bool bit = (temp & 1) != 0;     /* current bit */

        if (first_run) {
            last_bit = bit;
            run_len = 1;
            first_run = false;
        } else {
            if (bit == last_bit) {
                run_len++;
            } else {
                /* finalize previous run */
                if (run_len & 1) has_odd = true; else has_even = true;
                last_bit = bit;
                run_len = 1;
            }
        }
        temp = temp >> 1;              /* shift right to next bit */
    }

    /* record the final run */
    if (run_len & 1) has_odd = true; else has_even = true;

    return has_odd && has_even;
}

/* Return the nth term (1‑based) of OEIS sequence A160531. */
seq_t a(idx_t n)
{
    idx_t count = 0;          /* how many valid numbers found so far */
    seq_t candidate = 1;      /* current integer to test */

    while (count < n) {
        if (has_both_runs(candidate)) {
            count++;
            if (count == n) return candidate;
        }
        candidate = candidate + 1;   /* move to next integer */
    }

    /* control never reaches here */
    return candidate;
}
