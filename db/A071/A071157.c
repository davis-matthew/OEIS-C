/*
 * A071157  The zero-free, right-to-left factorial walk encoding for each
 *          rooted plane tree encoded by A014486. Sequence A071155 shown with
 *          factorial expansion (A007623).
 * https://oeis.org/A071157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(0)=0; for n≥1, generate the nth zero‑free * factorial‑walk
 *            decimal string. This is equivalent to the * OEIS definition of
 *            A071157. The algorithm enumerates strings * by length using
 *            Catalan numbers, finds the rank within the block, * and un‑ranks
 *            using dynamic programming over admissible digits
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* external integer types supplied by the build */
extern seq_t a(idx_t n);

/* maximum length needed for n up to about 10 000   (Catalan(14)≈2.6·10⁶)   */
#define MAXL 64

seq_t a(idx_t n)
{
    /* The null tree (empty sequence). */
    if (n == 0)
        return 0u;

    /* 1. Find the length L of the n-th string.
     *    Σ_{i=1..L} Catalan(i)  ≥  n.               */
    seq_t n_seq = (seq_t)n;
    seq_t cum = 0u;           /* cumulative count of sequences up to current length */
    seq_t cat_prev = 1u;      /* C0 = 1  (used to compute C1,C2,… by recurrence) */
    seq_t cat_cur = 0u;
    idx_t L = 0;

    for (L = 1; L <= MAXL; ++L)
    {
        /* Catalan recurrence: Cn = C(n-1) * (4n-2) / (n+1)     */
        seq_t mult = ((seq_t)L * 4u) - 2u;
        cat_cur = cat_prev * mult / (seq_t)(L + 1u);
        cat_prev = cat_cur;
        cum += cat_cur;
        if (cum >= n_seq)
            break;
    }

    /* 2. Rank k (0‑based) of the desired string inside the block of length L. */
    seq_t prev_cum = cum - cat_cur;          /* sum of previous blocks   */
    seq_t k = n_seq - prev_cum - 1u;          /* 0‑based index in block */

    /* 3. Pre‑compute the number of completions for each suffix. */
    /* memo[pos][prev] = #completions from position pos (0‑rightmost) to the left,
       given that the previous (right‑side) digit is 'prev'.             */
    seq_t memo[MAXL + 1][MAXL + 2];
    memset(memo, 0, sizeof(memo));

    /* Base case: beyond the leftmost digit there is exactly one completion. */
    for (idx_t prev = 1; prev <= L + 1; ++prev)
        memo[L][prev] = 1u;

    /* Compute memo in reverse (from L‑1 down to 0). */
    for (int pos = (int)L - 1; pos >= 0; --pos)
    {
        for (idx_t prev = 1; prev <= L + 1; ++prev)
        {
            seq_t sum = 0u;
            int limit = (pos + 1 < prev + 1) ? (pos + 1) : (prev + 1);
            for (int cand = 1; cand <= limit; ++cand)
                sum += memo[pos + 1][cand];
            memo[pos][prev] = sum;
        }
    }

    /* 4. Un‑rank to obtain the digits (rightmost digit is always 1).      */
    uint8_t digits[MAXL];
    memset(digits, 1, L);               /* all digits start at 1            */
    idx_t prev_digit = 1;

    for (int pos = 1; pos < (int)L; ++pos)
    {
        int limit = (pos + 1 < prev_digit + 1) ? (pos + 1) : (prev_digit + 1);
        for (int cand = 1; cand <= limit; ++cand)
        {
            seq_t cnt = memo[pos + 1][cand];
            if (k < cnt)
            {
                digits[pos] = (uint8_t)cand;
                prev_digit = cand;
                break;
            }
            k -= cnt;
        }
    }

    /* 5. Convert the digit array into the decimal integer.                */
    seq_t result = 0u;
    for (int i = (int)L - 1; i >= 0; --i)
        result = result * 10u + (seq_t)digits[i];

    return result;
}
