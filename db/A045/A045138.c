/*
 * A045138  Numbers whose base-4 representation contains exactly one 2 and no
 *          3's.
 * https://oeis.org/A045138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Count, by length L, the numbers in base‑4 that contain
   exactly one digit ‘2’ and no digit ‘3’.  The count is
   1 if L=1, and (L+1)*2^(L-2) if L≥2.
   Find the length L that contains the n‑th such number, then construct
   the k‑th (0‑based) number of that length in lexicographic order. */

#include <stdbool.h>

/* shift 2^n for seq_t */
static inline seq_t pow2_seq(idx_t n)
{
    seq_t r = 1;
    for (idx_t i = 0; i < n; ++i)
        r <<= 1;
    return r;
}

/* count of admissible numbers of length L (base‑4) */
static inline seq_t count_with_len(idx_t L)
{
    if (L == 1)
        return 1;
    return (seq_t)(L + 1) * pow2_seq(L - 2);
}

/* count of strings for remaining positions
   rem   – number of positions left
   placed – true if the 2 has already been used in the prefix
*/
static inline seq_t count_remaining(idx_t rem, bool placed)
{
    if (rem == 0)
        return placed ? 1 : 0;
    if (placed)
        return pow2_seq(rem);                  /* all 0 or 1 */
    return (seq_t)rem * pow2_seq(rem - 1);      /* one 2 somewhere */
}

/* construct k‑th (0‑based) string of length L in lexicographic order */
static seq_t build_kth(idx_t L, seq_t k0)
{
    seq_t value = 0;
    bool twoPlaced = false;

    for (idx_t pos = 0; pos < L; ++pos)
    {
        idx_t rem   = L - pos - 1;
        bool first  = (pos == 0);

        for (int d = 0; d <= 2; ++d)
        {
            if (first && d == 0)               /* leading zero not allowed */
                continue;
            if (d == 2 && twoPlaced)           /* only one 2 allowed */
                continue;

            bool newTwo = twoPlaced || (d == 2);
            seq_t cnt = count_remaining(rem, newTwo);

            if (k0 >= cnt)
            {
                k0 -= cnt;
                continue;
            }

            value = value * 4 + (seq_t)d;
            twoPlaced = newTwo;
            break;
        }
    }
    return value;
}

seq_t a(idx_t n)
{
    /* find the length L that contains the n‑th number */
    seq_t remaining = (seq_t)n;
    idx_t L = 1;
    for (;; ++L)
    {
        seq_t cntL = count_with_len(L);
        if (remaining <= cntL)
            break;
        remaining -= cntL;
    }

    seq_t k0 = remaining - 1;              /* 0‑based within this length */
    return build_kth(L, k0);
}
