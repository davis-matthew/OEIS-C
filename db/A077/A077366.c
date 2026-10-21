/*
 * A077366  Triangular numbers whose internal digits form a triangular number.
 *          Or triangular number such that deleting the MSD and LSD leaves a
 *          triangular number.
 * https://oeis.org/A077366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   interior_digits, isqrt_seq, seq_digits, seq_pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077366: Triangular numbers whose internal digits form a triangular number */

#include <stdbool.h>
#include "../utils/isqrt_seq.h"
#include "../utils/interior_digits.h"
#include "../utils/seq_digits.h"
#include "../utils/seq_pow10.h"

/* Return true iff x is a triangular number (x = m(m+1)/2 for some m). */
static bool is_triangular(seq_t x)
{
    seq_t v = 8 * x + 1;
    seq_t s = isqrt_seq(v);
    return s * s == v;
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t tri = 0;           /* T_0 = 0 */
    for (idx_t k = 0;; ++k)
    {
        if (is_triangular(interior_digits(tri)))
        {
            ++count;
            if (count == n)
                return tri;
        }
        tri += (k + 1);      /* T_{k+1} = T_k + (k+1) */
    }
}
