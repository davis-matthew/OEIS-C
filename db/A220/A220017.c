/*
 * A220017  Relative probability for a four-player Old Maid match to last for
 *          exactly n games until a player has been trolled exactly three
 *          times.
 * https://oeis.org/A220017
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = A216074(n) / 32 with A216074(n) counting length‑9 base‑4
 *            sequences whose first digit to reach 3 occurrences is exactly at
 *            position n (3 ≤ n ≤ 9). The computation uses the combinatorial
 *            expression A216074(n) = (4 * Σ_{x1,x2,x3 ∈ [0,2], x1+x2+x3=n-3}
 *            (n‑1)! / (2! · x1! · x2! · x3!)) · 4^(9‑n)
 * Range:     u8 none, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const seq_t fact[9] = {
    1ULL,      /* 0! */
    1ULL,      /* 1! */
    2ULL,      /* 2! */
    6ULL,      /* 3! */
    24ULL,     /* 4! */
    120ULL,    /* 5! */
    720ULL,    /* 6! */
    5040ULL,   /* 7! */
    40320ULL   /* 8! */
};

seq_t a(idx_t n)
{
    /* The sequence is defined only for 3 ≤ n ≤ 9 */
    if (n < 3 || n > 9)
        return 0;

    /* Remaining number of symbols to distribute in the first (n‑1)
       positions when the digit that will reach 3 already has count 2. */
    idx_t remaining = n - 3;
    seq_t sum = 0;

    /* Enumerate all triples (x1,x2,x3) with each ∈[0,2] and sum = remaining. */
    for (idx_t x1 = 0; x1 <= 2; ++x1) {
        if (x1 > remaining) continue;
        for (idx_t x2 = 0; x2 <= 2; ++x2) {
            idx_t s12 = x1 + x2;
            if (s12 > remaining) continue;
            idx_t x3 = remaining - s12;
            if (x3 > 2) continue;

            /* Multinomial coefficient: (n‑1)! / (2! · x1! · x2! · x3!) */
            seq_t denom = (seq_t)2 * fact[x1] * fact[x2] * fact[x3];
            seq_t term  = fact[n - 1] / denom;
            sum += term;
        }
    }

    /* 4 choices for the digit that will achieve its third occurrence. */
    sum *= 4;

    /* 4^(9‑n) factor for the remaining (9‑n) positions. */
    seq_t pow = 1;
    for (idx_t i = 0; i < 9 - n; ++i)
        pow *= 4;

    /* Divide by 32 to obtain the final probability value.  The division
       is exact for all 3 ≤ n ≤ 9. */
    return (sum * pow) / 32;
}
