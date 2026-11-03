/*
 * A164713  A positive integer n is included if all runs of 0's in binary n
 *          are of the same length, and if all runs of 1's in binary n are of
 *          the same length.
 * https://oeis.org/A164713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A164713: nth positive integer whose binary runs of 0s and 1s
   all have the same length. Brute‑force enumeration is used. */

#include <stdbool.h>

static bool has_uniform_runs(seq_t x)
{
    if (x == (seq_t)0) return false;

    /* Determine bit length of x */
    idx_t bitlen = 0;
    seq_t tmp = x;
    while (tmp) { tmp >>= 1; ++bitlen; }

    bool in_one_run = true;   /* current run is of 1s initially */
    idx_t run_len = 0;
    idx_t len_one = 0, len_zero = 0;  /* lengths seen so far */

    for (idx_t i = bitlen; i-- > 0; ) {
        bool bit = ((x >> i) & (seq_t)1) != 0;
        if (bit) {  /* bit is 1 */
            if (!in_one_run) {
                if (len_zero == 0)
                    len_zero = run_len;
                else if (len_zero != run_len)
                    return false;
                run_len = 0;
                in_one_run = true;
            }
            ++run_len;
        } else {    /* bit is 0 */
            if (in_one_run) {
                if (len_one == 0)
                    len_one = run_len;
                else if (len_one != run_len)
                    return false;
                run_len = 0;
                in_one_run = false;
            }
            ++run_len;
        }
    }

    /* Final run */
    if (run_len) {
        if (in_one_run) {
            if (len_one == 0)
                len_one = run_len;
            else if (len_one != run_len)
                return false;
        } else {
            if (len_zero == 0)
                len_zero = run_len;
            else if (len_zero != run_len)
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = (seq_t)1;

    while (true) {
        if (has_uniform_runs(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
}
