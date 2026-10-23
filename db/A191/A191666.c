/*
 * A191666  Dispersion of A042964 (numbers congruent to 2 or 3 mod 4), by
 *          antidiagonals.
 * https://oeis.org/A191666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 137, u32 n <= 529, u64 n <= 2081,
 *            u128 n <= 5050, big any n
 * Helpers:   diag_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A191666: Dispersion of A042964 (numbers ≡ 2 or 3 mod 4) by antidiagonals.
   Algorithm: form an infinite matrix.  Row i starts with
     s_i = 2·i–1  if i is odd,
            2·i      if i is even.
   Subsequent entries in the row use
     f(x) = 2·x          if x is odd (x≡1 (mod 4)),
             2·x – 1        if x is even (x≡0 (mod 4)).
   The matrix is read by successive antidiagonals (Cantor pairing) and
   a(n) is the n‑th element (1‑based).  This reproduces the OEIS table:
   1,2,4,3,7,5,6,14,10,8,11,27,19,15,9,22,54,38,30,18,12,43,107,75,… */

#include <stdint.h>
#include <math.h>
#include "../utils/diag_coords.h"

seq_t
a(idx_t n)
{
    if (n == 0) {                 /* not required by spec, but guard */
        return (seq_t)0;
    }

    idx_t i, j;
    diag_coords(n, &i, &j);

    /* Starting value of the row i. */
    seq_t val;
    if (i & 1)                     /* i odd → 2*i‑1 */
        val = (seq_t)(2 * i - 1);
    else                           /* i even → 2*i  */
        val = (seq_t)(2 * i);

    /* Apply the row‑recurrence (j‑1) times. */
    for (idx_t step = 1; step < j; ++step) {
        if ((val & 1) == 0)         /* even: 2*val – 1 */
            val = val * 2 - 1;
        else                        /* odd : 2*val   */
            val = val * 2;
    }
    return val;
}
