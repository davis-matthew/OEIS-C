/*
 * A044875  Numbers having, in base 4, (sum of even run lengths)=(sum of odd
 *          run lengths).
 * https://oeis.org/A044875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose base‑4 representation has
 *            equal total lengths of runs of even length and runs of odd
 *            length
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool equal_run_parity(seq_t x)
{
    seq_t even_sum = 0, odd_sum = 0;
    seq_t temp = x;
    while (temp != 0) {
        seq_t digit = temp % 4;
        seq_t run_len = 0;
        while (temp != 0 && (temp % 4) == digit) {
            temp /= 4;
            run_len++;
        }
        if ((run_len & 1) == 0)
            even_sum += run_len;
        else
            odd_sum += run_len;
    }
    return even_sum == odd_sum;
}

seq_t a(idx_t n)
{
    seq_t candidate = 0;
    idx_t found = 0;
    while (found < n) {
        candidate++;
        if (equal_run_parity(candidate))
            found++;
    }
    return candidate;
}
