/*
 * A045128  Numbers whose base-4 representation contains exactly three 1's and
 *          four 3's.
 * https://oeis.org/A045128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence definition: a(n) is the nth smallest integer whose base‑4 representation
   contains exactly three 1’s and four 3’s (no other digits).  The sequence has
   C(7,3)=35 terms. */
seq_t a(idx_t n)
{
    if (n == 0u) return 0;            /* undefined for n=0, return 0 */
    idx_t r = n - 1u;                 /* zero‑based rank */
    const idx_t total = 35u;          /* number of admissible 7‑digit patterns */
    if (r >= total) return 0;         /* out of range */

    idx_t ones_left = 3u;             /* digits equal to 1 that we still need */
    seq_t val = 0u;                   /* accumulated value in base‑4 */

    /* For each of the 7 positions (most significant first) decide 1 or 3. */
    for (idx_t rem = 7u; rem > 0u; --rem) {
        if (ones_left == 0u) {
            /* No more 1’s to place; all remaining digits are 3. */
            val = val * 4u + (seq_t)3u;
            continue;
        }

        /* Count how many patterns are possible if we put a 1 at this position. */
        idx_t ways_with_one = 0u;
        if (rem - 1u >= ones_left - 1u) {
            /* Compute binomial coefficient C(rem-1, ones_left-1). */
            idx_t k = ones_left - 1u;
            idx_t m = rem - 1u;
            if (k > m - k) k = m - k;           /* symmetry */
            idx_t res = 1u;
            for (idx_t i = 1u; i <= k; ++i)
                res = res * (m - k + i) / i;
            ways_with_one = res;
        }

        if (r < ways_with_one) {
            /* Place a 1 at this position. */
            val = val * 4u + (seq_t)1u;
            --ones_left;
        } else {
            /* Place a 3 at this position and skip the skipped patterns. */
            val = val * 4u + (seq_t)3u;
            r -= ways_with_one;
        }
    }

    return val;
}
