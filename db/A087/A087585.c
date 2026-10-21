/*
 * A087585  a(1) = 12; a(n) = smallest multiple of a(n-1), not == 0 (mod 10)
 *          beginning with the digit reversal of a(n-1).
 * https://oeis.org/A087585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   pow10, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A087585
   a(1) = 12
   a(n) = smallest multiple of a(n-1) that does not end with 0
          and whose decimal representation begins with the digit
          reversal of a(n-1).  */
#include "../utils/rev_decimal.h"
#include "../utils/pow10.h"

static seq_t next_term(seq_t prev)
{
    seq_t revPrev = rev_decimal(prev);

    for (int d = 0; ; ++d) {
        seq_t pow10_d = pow10((unsigned int)d);
        seq_t L = revPrev * pow10_d;
        seq_t U = (revPrev + 1) * pow10_d - 1;

        /* ceil( L / prev ) */
        seq_t k = (L + prev - 1) / prev;

        while (1) {
            seq_t M = k * prev;
            if (M > U)   /* no solution in this digit length */
                break;

            if ((M % (seq_t)10) != 0)
                return M;            /* found the required term */

            ++k;                     /* try next multiple */
        }
    }
    /* unreachable */
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, return 0 */
    seq_t current = 12;            /* a(1) */
    if (n == 1) return current;

    for (idx_t i = 2; i <= n; ++i) {
        current = next_term(current);
    }
    return current;
}
