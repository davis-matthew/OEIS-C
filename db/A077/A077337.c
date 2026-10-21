/*
 * A077337  Numbers k such that k and R(k) both are squarefree where R(n)
 *          (A004086) is the digit reversal of n.
 * https://oeis.org/A077337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 121, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_sqfree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077337: numbers k such that k and its decimal reversal R(k) are both square‑free */

#include <stdbool.h>
#include "../utils/is_sqfree.h"

/* Compute the decimal reversal of an idx_t number, dropping leading zeros. */
static idx_t rev_decimal(idx_t x)
{
    idx_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + (x % 10);
        x /= 10;
    }
    return rev;
}

/* Return the n-th term of the sequence (1‑based offset). */
extern seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* for safety – offset is 1 */

    idx_t found = 0;
    idx_t k = 0;

    while (found < n) {
        ++k;                            /* candidate k */

        if (!is_sqfree(k))              /* k itself not square‑free */
            continue;

        idx_t r = rev_decimal(k);       /* reversed number */
        if (!is_sqfree(r))              /* reversal not square‑free */
            continue;

        ++found;                        /* one more term found */
    }

    return (seq_t)k;                  /* cast to the sequence type */
}
