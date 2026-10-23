/*
 * A044997  Numbers whose base-3 representation contains exactly one 0 and
 *          three 2's.
 * https://oeis.org/A044997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1638, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   choose_n_k, count_of_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044997: Numbers whose base‑3 representation contains exactly
   one 0 and three 2's.
   Implementation: Build the n‑th number lexicographically by
   constructing its base‑3 digits from most significant to least, using
   combinatorial counts to skip whole blocks. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/choose_n_k.h"
#include "../utils/count_of_length.h"

/* Number of completions for the remaining 'rem' positions, given
   twos still needed and whether the unique zero has yet to be placed. */
static idx_t count_completions(idx_t rem, idx_t twos_rem, bool zero_needed)
{
    if (twos_rem > rem) return 0;

    if (!zero_needed)                  /* zero already used */
    {
        return choose_n_k(rem, twos_rem);
    }

    /* zero still to be placed somewhere among the remaining positions */
    if (rem == 0) return 0;             /* cannot place zero later */
    if (twos_rem > rem - 1) return 0;   /* not enough spots for twos after zero */

    return rem * choose_n_k(rem - 1, twos_rem);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* behavior for n < 1 is irrelevant */

    /* Step 1: locate the length L of the n‑th number. */
    idx_t L = 4;
    idx_t cumulative = 0;
    while (1)
    {
        idx_t cnt = count_of_length(L);
        if (cumulative + cnt >= n) break;
        cumulative += cnt;
        ++L;
    }

    idx_t k = n - cumulative;           /* 1‑based index within length L */

    seq_t val = 0;
    bool zero_needed = true;             /* zero not placed yet */
    idx_t twos_needed = 3;               /* three twos still to place */

    /* Build digits from most significant to least. */
    for (idx_t pos = 0; pos < L; ++pos)
    {
        idx_t remaining = L - pos - 1;  /* positions after current */

        for (int d = 0; d <= 2; ++d)
        {
            /* Skip invalid digits */
            if (pos == 0 && d == 0) continue;          /* leading zero forbidden */
            if (!zero_needed && d == 0) continue;       /* only one zero allowed */
            if (d == 2 && twos_needed == 0) continue;   /* only three twos allowed */

            bool new_zero_needed = zero_needed && (d != 0);
            idx_t new_twos_needed = twos_needed - (d == 2 ? 1 : 0);

            idx_t cnt = count_completions(remaining, new_twos_needed, new_zero_needed);

            if (cnt < k)
            {
                k -= cnt;          /* skip this entire block */
                continue;
            }

            /* This digit is part of the desired number. */
            val = val * (seq_t)3 + (seq_t)d;
            zero_needed = new_zero_needed;
            twos_needed = new_twos_needed;
            break;                  /* proceed to next position */
        }
    }

    return val;
}
