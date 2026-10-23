/*
 * A037355  Numbers whose base-4 and base-8 expansions have no digits in
 *          common.
 * https://oeis.org/A037355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS A037355 sequence:  numbers whose base‑4 and base‑8
 * expansions have no common digits.  The sequence is memoised in a
 * static array so that successive calls (in any order) are fast.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper: test whether a number has no common digits in its base‑4
 * and base‑8 representations.  The function works for any integer
 * type on which the operators % and / are defined for the constants
 * 4 and 8. */
static bool no_common_digits(seq_t x) {
    unsigned mask4 = 0, mask8 = 0;
    seq_t t = x;

    /* Base‑4 digits */
    if (t == 0)
        mask4 = 1;            /* digit 0 */
    while (t != 0) {
        unsigned d = (unsigned)(t % 4);
        mask4 |= 1u << d;
        t /= 4;
        if ((mask4 & mask8) != 0)
            return false;    /* early exit */
    }

    /* Base‑8 digits */
    t = x;
    if (t == 0)
        mask8 = 1;            /* digit 0 */
    while (t != 0) {
        unsigned d = (unsigned)(t % 8);
        mask8 |= 1u << d;
        if ((mask4 & mask8) != 0)
            return false;    /* early exit */
        t /= 8;
    }

    return (mask4 & mask8) == 0;
}

/* Main sequence function.  It uses a static array to cache already
 * generated terms and a counter to keep track of the next candidate
 * number to test. */
seq_t a(idx_t n) {
    /* Static memoisation buffer */
    static seq_t *seq   = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static seq_t next_candidate = 1;

    if (n == 0)
        return 0;  /* undefined for the problem, but return 0 safely */

    /* Expand buffer until we have at least n terms */
    while (seq_len < (size_t)n) {
        /* Grow the array if necessary */
        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 4;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp)
                return 0; /* allocation failed */
            seq = tmp;
            seq_cap = new_cap;
        }

        /* Find next valid number */
        while (!no_common_digits(next_candidate)) {
            next_candidate++;
        }
        seq[seq_len++] = next_candidate;
        next_candidate++;
    }

    return seq[n - 1];
}
