/*
 * A050278  Pandigital numbers: numbers containing the digits 0-9. Version 1:
 *          each digit appears exactly once.
 * https://oeis.org/A050278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   a(n) is the nth 0‑9 pandigital number (digits 0‑9 each exactly
 *            once) in increasing numeric order with no leading zero
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* factorials up to 9! */
static const idx_t fact[10] = {
    1ULL, 1ULL, 2ULL, 6ULL, 24ULL,
    120ULL, 720ULL, 5040ULL, 40320ULL, 362880ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* outside the OEIS offset */

    /* rank in zero‑based indexing among all valid numbers */
    idx_t rank   = n - 1;
    idx_t block  = fact[9];              /* 9! permutations for each choice of the first digit */

    idx_t first_idx   = rank / block;    /* 0 … 8 correspond to first digits 1 … 9 */
    unsigned first_digit = 1 + (unsigned)first_idx;

    idx_t rem = rank % block;

    bool used[10] = {false};
    used[first_digit] = true;

    seq_t value = (seq_t)first_digit;

    /* Build the remaining 9 positions */
    for (int pos = 1; pos < 10; ++pos) {
        idx_t perms_left = fact[9 - pos];   /* permutations of the remaining slots */

        for (int d = 0; d <= 9; ++d) {
            if (used[d]) continue;

            if (rem < perms_left) {
                value = value * 10 + (seq_t)d;
                used[d] = true;
                break;
            } else {
                rem -= perms_left;
            }
        }
    }

    return value;
}
