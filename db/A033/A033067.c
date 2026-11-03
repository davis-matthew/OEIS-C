/*
 * A033067  Numbers whose base-16 representation Sum_{i=0..m} d(i)*16^i has
 *          odd d(m-i) for all odd i.
 * https://oeis.org/A033067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A033067
 * Numbers whose base‑16 representation Sum_{i=0..m} d(i)*16^i
 * has odd d(m-i) for all odd i.
 *
 * The function returns the n‑th term (n≥1) of the sequence.
 * It is a straightforward O(n·average‑digits) generator;
 * no memoisation is required for n≤~10000 (runtime <<2 s).
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper: test whether a value satisfies the A033067 condition */
static bool is_a033067(seq_t x)
{
    if (!x)  /* skip 0 – the sequence starts at 1 */
        return false;

    /* Store hex digits (least significant first) */
    unsigned char d[256];
    size_t len = 0;

    seq_t t = x;
    while (t) {
        d[len++] = (unsigned char)(t % (seq_t)16);
        t /= (seq_t)16;
    }

    /* For each position j from 0..len-1 (LSB index),
     * if (len-1-j) is odd then digit d[j] must be odd. */
    for (size_t j = 0; j < len; ++j) {
        if (((len - 1 - j) & 1) && ((d[j] & 1) == 0))
            return false;
    }
    return true;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for n<1, but safe to return 0 */

    idx_t found = 0;
    seq_t x = (seq_t)1;

    while (1) {
        if (is_a033067(x)) {
            ++found;
            if (found == n)
                return x;
        }
        ++x;
    }
}
