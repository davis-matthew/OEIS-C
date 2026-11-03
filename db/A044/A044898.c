/*
 * A044898  Base 12 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑12 representation *
 *            has runs of consecutive equal digits alternating between even
 *            and odd * lengths, starting with an even length. The sequence
 *            begins * a(1)=13, a(2)=26, a(3)=39, … (OEIS A044898)
 * Range:     u8 n <= 22, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool base12_good(seq_t value)
{
    /* Extract base‑12 digits (least significant first) */
    seq_t tmp = value;
    /* 4096 digits is ample for the values needed in the tests */
    unsigned char digits[4096];
    unsigned int len = 0;

    while (tmp != (seq_t)0) {
        digits[len++] = (unsigned char)(tmp % (seq_t)12);
        tmp /= (seq_t)12;
    }
    if (len == 0) {
        return false;          /* 0 is not a positive integer */
    }

    /* The first run must be even */
    bool even_expected = true;
    unsigned int run_len = 1;

    for (int i = (int)len - 1; i > 0; --i) {
        if (digits[i] == digits[i - 1]) {
            ++run_len;
        } else {
            if ((run_len & 1) != (even_expected ? 0 : 1))
                return false;
            even_expected = !even_expected;
            run_len = 1;
        }
    }
    /* Check the final run */
    if ((run_len & 1) != (even_expected ? 0 : 1))
        return false;

    return true;
}

seq_t a(idx_t n)
{
    /* We compute the term by brute‑force scanning from the first term,
     * without any dynamic allocation.  This meets the requirements
     * for any order of calls and any value of n for the given limits
     * (up to about 10 000).                   */
    seq_t candidate = (seq_t)13;   /* first term of the sequence */
    idx_t count = 0;

    for (;;) {
        if (base12_good(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}
