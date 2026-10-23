/*
 * A045086  Numbers whose base-4 representation contains no 1's and exactly
 *          one 2.
 * https://oeis.org/A045086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose base-4 representation contains no 1's and exactly
 *            one 2
 * Range:     u8 n <= 32, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   count_block2, count_with_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for 1 <= n. */
#include <stdbool.h>
#include "../utils/count_block2.h"
#include "../utils/count_with_length.h"

/* Count of strings of length rem over {0,2,3} with exactly one 2
   given whether a 2 has already been used. */
static idx_t count_remaining(idx_t rem, bool used2)
{
    if (rem == 0) return used2 ? 1 : 0;
    if (!used2)
        return rem * (((idx_t)1) << (rem - 1)); /* choose one 2 */
    return ((idx_t)1) << rem;                /* all 0/3 */
}

/* Generate the lexicographically k‑th string of length m
   over {0,2,3} containing exactly one 2. */
static void lower_digits(idx_t m, idx_t k, unsigned char *out)
{
    bool used2 = false;
    for (idx_t pos = 0; pos < m; ++pos)
    {
        idx_t rem = m - pos - 1;

        /* digit 0 */
        idx_t cnt0 = count_remaining(rem, used2);
        if (k < cnt0) { out[pos] = 0; continue; }
        k -= cnt0;

        /* digit 2 (if still unused) */
        if (!used2)
        {
            idx_t cnt2 = count_remaining(rem, true);
            if (k < cnt2) { out[pos] = 2; used2 = true; continue; }
            k -= cnt2;
        }

        /* digit 3 */
        out[pos] = 3;
        /* used2 stays unchanged */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined, but safe */

    /* Find the length L for which n lies. */
    idx_t L = 1;
    while (true)
    {
        idx_t cnt = count_with_length(L);
        if (n <= cnt) break;
        n -= cnt;
        ++L;
    }

    if (L == 1) return (seq_t)2;   /* only the single digit 2 */

    idx_t block2 = ((idx_t)1) << (L - 1); /* numbers with MSD = 2 */

    if (n <= block2)           /* MSD is 2 */
    {
        idx_t m = n - 1;       /* zero‑based index within this block */
        seq_t res = 2;
        for (idx_t i = 0; i < L - 1; ++i)
        {
            idx_t bit = (m >> (L - 2 - i)) & 1;
            seq_t digit = bit ? 3 : 0;
            res = res * 4 + digit;
        }
        return res;
    }
    else                        /* MSD is 3 */
    {
        n -= block2;           /* re‑index within the block with MSD 3 */
        idx_t k = n - 1;       /* zero‑based index */
        idx_t m = L - 1;       /* number of lower positions */
        unsigned char lower[64];
        lower_digits(m, k, lower);

        seq_t res = 3;
        for (idx_t i = 0; i < m; ++i)
            res = res * 4 + lower[i];
        return res;
    }
}
