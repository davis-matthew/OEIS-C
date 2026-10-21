/*
 * A077572  Nonsquarefree numbers obtained by repeating a single digit.
 * https://oeis.org/A077572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = nth non‑squarefree repdigit in base‑10
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

/* Computes the nth term (n ≥ 1). */
seq_t a(idx_t n)
{
    seq_t repunit = 1;          /* R(1) = 1 */
    unsigned int r_mod3 = 1 % 3;
    unsigned int r_mod7 = 1 % 7;
    idx_t count = 0;

    for (idx_t len = 1; ; ++len)
    {
        bool repunit_sqfree = is_squarefree(repunit);

        for (int d = 1; d <= 9; ++d)
        {
            bool include = false;

            if (!repunit_sqfree)
                include = true;
            else
            {
                switch (d)
                {
                case 4: case 8: case 9:
                    include = true;
                    break;
                case 3: case 6:
                    if (r_mod3 == 0) include = true;
                    break;
                case 7:
                    if (r_mod7 == 0) include = true;
                    break;
                default:
                    /* digits 1,2,5 do not introduce a square factor */
                    break;
                }
            }

            if (include)
            {
                ++count;
                if (count == n)
                    return repunit * (seq_t)d;
            }
        }

        /* Prepare for next length: R(L+1) = 10*R(L) + 1 */
        repunit = repunit * (seq_t)10 + (seq_t)1;
        r_mod3 = (r_mod3 * 10 + 1) % 3;   /* 10 ≡ 1 (mod 3) */
        r_mod7 = (r_mod7 * 10 + 1) % 7;
    }
}
