/*
 * A074134  First column of triangle A074135.
 * https://oeis.org/A074134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(1)=1. For n>1, let E(k)=1 if k equals 3^m-2 or 3^m-1 for some
 *            m≥1, otherwise 0. Then a(n) = a(n-1) + (n-1) + E(n) - E(n-1).
 *            This recurrence reproduces OEIS A074134
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_excep, is_pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_excep.h"
#include "../utils/is_pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset=1 guarantees n≥1, but guard. */
    seq_t result = 1;              /* a(1) = 1 */
    bool prev_exc = is_excep(1);   /* actually true for n=1 */

    for (idx_t i = 2; i <= n; ++i) {
        bool curr_exc = is_excep(i);
        seq_t diff = (seq_t)(i - 1);          /* base difference m-1 */

        if (curr_exc && !prev_exc) diff++;    /* first of an exceptional pair */
        else if (!curr_exc && prev_exc) diff--;/* after an exceptional pair */

        result += diff;
        prev_exc = curr_exc;
    }
    return result;
}
