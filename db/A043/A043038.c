/*
 * A043038  Base-10 palindromes that starts with 3.
 * https://oeis.org/A043038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 122, u32 n <= 2222, u64 n <= 2222,
 *            u128 n <= 2222, big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth base‑10 palindrome that starts with 3. */
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* Handle the trivial single‑digit case directly. */
    if (n == 1) {
        return seq_from_str("3");
    }

    idx_t cumulative = 1;                 /* count up to length 1 (the "3") */
    for (idx_t d = 2; ; ++d) {
        /* Count palindromes of length d that start with 3. */
        idx_t k = d / 2;                 /* length of the left half */
        idx_t base = 1;
        for (idx_t i = 0; i < k - 1; ++i) base *= 10;   /* 10^(k-1) */

        idx_t count = (d % 2 == 0) ? base : base * 10;   /* even/odd */

        if (cumulative + count >= n) {
            /* Found the length containing the nth palindrome. */
            idx_t within = n - cumulative - 1;          /* zero‑based index */

            /* Prepare the left half: '3' followed by k‑1 digits. */
            char left[16];
            left[0] = '3';
            idx_t left_idx = (d % 2 == 0) ? within : within / 10;
            idx_t tmp = left_idx;
            for (int i = (int)(k - 1); i >= 1; --i) {
                left[i] = '0' + (tmp % 10);
                tmp /= 10;
            }

            /* Build the complete palindrome string. */
            char full[32];
            int pos = 0;
            /* Copy left half */
            for (int i = 0; i < (int)k; ++i) full[pos++] = left[i];
            /* Middle digit if odd length */
            if (d % 2 == 1) {
                full[pos++] = '0' + (within % 10);
            }
            /* Append reverse of left half */
            for (int i = (int)k - 1; i >= 0; --i) full[pos++] = left[i];
            full[pos] = '\0';

            return seq_from_str(full);
        }
        cumulative += count;
    }
    /* Unreachable but keeps the compiler happy. */
    return 0;
}
