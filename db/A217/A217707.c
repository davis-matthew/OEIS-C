/*
 * A217707  Numbers k such that both 4*k-1 and 4*k+1 are composite.
 * https://oeis.org/A217707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A217707: numbers k such that both 4*k-1 and 4*k+1 are composite. */
/* Uses a simple search, testing each candidate sequentially. */

#include "../utils/isPrimeGeneric.h"
#include <stdbool.h>

static bool k_is_valid(idx_t k)
{
    seq_t a = (seq_t)4 * k - 1;
    seq_t b = (seq_t)4 * k + 1;
    /* composite means >1 and not prime */
    return !isPrimeGeneric(a) && !isPrimeGeneric(b);
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 2;         /* k=1 gives 3 and 5, both prime – skip */
    for (;; ++k)
    {
        if (k_is_valid(k))
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
