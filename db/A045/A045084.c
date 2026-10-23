/*
 * A045084  Numbers whose base-4 representation contains exactly four 0's and
 *          three 3's.
 * https://oeis.org/A045084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 295, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A045084:
   Numbers whose base‑4 representation contains exactly four ‘0’ digits
   and three ‘3’ digits.
   The function returns the n‑th term (1‑based) of this sequence. */
       
#include <stddef.h>
#include <stdint.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k) */
#include "../utils/pow2.h"       /* seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* sequence undefined for n < 1 */

    /* 1. Find the length L of the base‑4 representation that contains
       the n‑th valid number.  Start from the minimal possible length. */
    idx_t  L        = 7;                 /* minimal length (4 zeros + 3 threes) */
    idx_t  remain   = n;                 /* index to locate within the whole sequence */

    while (1) {
        /* total number of L‑digit base‑4 strings with 4 zeros and 3 threes */
        seq_t tot      = binomial(L, 4);
        tot           = tot * binomial(L - 4, 3);
        tot           = tot * pow2(L - 7);

        /* subtract strings that start with a leading zero */
        seq_t leadz   = binomial(L - 1, 3);
        leadz        = leadz * binomial(L - 4, 3);
        leadz        = leadz * pow2(L - 7);

        seq_t count_L = tot - leadz;     /* valid numbers of length L */

        seq_t rem_seq = (seq_t)remain;
        if (rem_seq <= count_L) {        /* target lies within this length   */
            break;                       /* L is already correct            */
        }
        /* move to next length */
        remain -= (idx_t)count_L;         /* count_L fits in idx_t (see note) */
        L++;
    }

    /* 2. Determine the zero‑based index of the target within length L. */
    idx_t  index = remain - 1;            /* zero‑based rank within this block */

    /* 3. Unrank the number digit by digit (lexicographic = numeric order). */
    idx_t  zeros_left  = 4;                /* still needed zeros               */
    idx_t  threes_left = 3;                /* still needed threes              */
    seq_t  value       = 0;                /* constructed integer              */

    for (idx_t pos = 0; pos < L; ++pos) {
        int chosen = -1;
        for (int d = 0; d <= 3; ++d) {
            if (pos == 0 && d == 0) continue;                  /* leading zero forbidden */

            /* Check availability if d is 0 or 3 */
            if (d == 0 && zeros_left == 0) continue;
            if (d == 3 && threes_left == 0) continue;

            /* Remaining counts after tentatively placing digit d */
            idx_t z = zeros_left;
            idx_t t = threes_left;
            if (d == 0)  z--;
            else if (d == 3) t--;

            idx_t rem_pos = L - pos - 1;                      /* positions left after this digit */
            if (z > rem_pos || t > rem_pos - z) continue;    /* impossible state */

            /* Number of completions from this state */
            seq_t cnt = binomial(rem_pos, z);
            cnt      = cnt * binomial(rem_pos - z, t);
            idx_t others = rem_pos - z - t;
            cnt        = cnt * pow2(others);

            seq_t idx_seq = (seq_t)index;
            if (idx_seq < cnt) {                            /* digit belongs here */
                chosen      = d;
                zeros_left  = z;
                threes_left = t;
                /* accumulate value */
                value = value * (seq_t)4 + (seq_t)d;
                break;
            } else {
                index -= (idx_t)cnt;                        /* skip this block */
            }
        }
        /* chosen must always be found */
    }

    return value;
}
