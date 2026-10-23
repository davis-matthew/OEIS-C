/*
 * A043401  Numbers whose base-7 representation contains exactly one 2.
 * https://oeis.org/A043401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer whose base‑7 representation contains
 *            exactly one digit 2
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Pre‑compute powers of 6; 30 is more than enough for n up to even 10⁶. */
static seq_t _pow6[30];

/* Count of completions for a suffix of length r, given whether the
   digit 2 has already been placed in the prefix. */
static seq_t _count_suffix(idx_t r, bool placed)
{
    if (r == 0)
        return placed ? (seq_t)1 : (seq_t)0;          /* empty suffix */
    if (placed)
        return _pow6[r];                             /* 6ⁿ possibilities */
    return (seq_t)r * _pow6[r-1];                    /* choose one 2 in r positions,
                                                       other digits avoid 2 */
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                             /* undefined for n < 1 */

    /* compute powers of 6 up to the bound */
    _pow6[0] = (seq_t)1;
    for (int i = 1; i < 30; ++i)
        _pow6[i] = _pow6[i-1] * (seq_t)6;

    seq_t idx_seq = (seq_t)n - (seq_t)1;              /* zero‑based index */
    seq_t cumulative = (seq_t)0;
    idx_t L = 1;

    /* find the length of the base‑7 representation */
    while (1) {
        seq_t countL;
        if (L == 1)
            countL = (seq_t)1;
        else
            countL = _pow6[L-1] + (seq_t)(L-1) * (seq_t)5 * _pow6[L-2];

        if (cumulative + countL > idx_seq)
            break;
        cumulative += countL;
        ++L;
    }

    seq_t k = idx_seq - cumulative;                  /* position within words
                                                       of length L */

    /* Build the number in base 7. */
    seq_t result = (seq_t)0;
    bool placed = false;                              /* has a 2 been used? */
    idx_t remaining = L - 1;                          /* digits after the current one */

    /* choose the leading digit (cannot be 0) */
    for (int d = 1; d <= 6; ++d) {
        bool new_placed = placed || (d == 2);
        seq_t cnt = _count_suffix(remaining, new_placed);
        if (k < cnt) {
            result = (seq_t)d;
            placed = new_placed;
            break;
        }
        k -= cnt;
    }

    /* choose the remaining digits */
    for (idx_t pos = 1; pos < L; ++pos) {
        remaining = L - 1 - pos;
        for (int d = 0; d <= 6; ++d) {
            if (!placed && d == 2) {
                /* 2 not yet used – allowed */
            } else if (placed && d == 2) {
                /* 2 already used – cannot place again */
                continue;
            }
            bool new_placed = placed || (d == 2);
            seq_t cnt = _count_suffix(remaining, new_placed);
            if (k < cnt) {
                result = result * (seq_t)7 + (seq_t)d;
                placed = new_placed;
                break;
            }
            k -= cnt;
        }
    }

    return result;
}
