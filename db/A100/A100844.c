/*
 * A100844  Smallest m greater than n such that m^2 contains n^2 in its
 *          decimal representation.
 * https://oeis.org/A100844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_to_dec_string
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A100844 – Smallest m > n such that m^2 contains n^2 as a decimal substring. */

#include <string.h>   /* for strstr */
#include "../utils/seq_to_dec_string.h"

/*  Return a(n).  */
seq_t a(idx_t n)
{
    /* n² */
    seq_t n_sq = (seq_t)n * ((seq_t)n);
    char n_str[64];
    seq_to_dec_string(n_sq, n_str, sizeof(n_str));

    seq_t m = (seq_t)n + 1;
    for (;;) {
        seq_t m_sq = m * m;
        char m_str[64];
        seq_to_dec_string(m_sq, m_str, sizeof(m_str));
        if (strstr(m_str, n_str) != NULL) {
            return m;
        }
        ++m;
        /* The problem guarantees that a(n) ≤ 10·n for n > 0,
           so the loop will terminate quickly. For n = 0
           the sequence value is 10. */
    }
}
