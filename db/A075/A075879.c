/*
 * A075879  The (10^n)-th odd-digit number.
 * https://oeis.org/A075879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A014261(10^n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* target rank (1‑based) */
    seq_t rank = pow10(n);

    /* find the smallest L such that Σ5^k (k=1..L) ≥ rank */
    seq_t pow5   = 5;    /* 5^1 */
    seq_t cumul  = 5;    /* Σ_{k=1}^1 5^k */
    idx_t  L     = 1;    /* current length */

    while (cumul < rank) {
        pow5   *= 5;       /* 5^{L+1} */
        cumul  += pow5;   /* Σ_{k=1}^{L+1} 5^k */
        L++;
    }

    /* offset within the L‑digit block (0‑based) */
    seq_t cumul_prev = cumul - pow5;          /* Σ_{k=1}^{L-1} 5^k */
    seq_t offset     = rank - cumul_prev - 1; /* 0 ≤ offset < 5^L */

    /* construct the L‑digit odd‑digit number */
    seq_t result   = 0;
    seq_t pow5_cur = pow5; /* 5^L */

    for (idx_t i = 0; i < L; ++i) {
        pow5_cur /= 5;                /* 5^{L-i-1} */
        seq_t digit = offset / pow5_cur;   /* 0..4 */
        offset %= pow5_cur;
        result = result * 10 + (seq_t)(1 + 2 * digit);  /* map 0..4 → 1,3,5,7,9 */
    }

    return result;
}
