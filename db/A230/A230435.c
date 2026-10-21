/*
 * A230435  Triangle by rows, A001047 convolved with A000079.
 * https://oeis.org/A230435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) with r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r(r+1)/2, T(r,k) = 3^(k+1)*2^(r-k) - 2^(r+1)
 * Range:     u8 n <= 17, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the built‑in utilities for powering */
#include <stdint.h>
#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while (1) {
        unsigned __int128 cnt = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (cnt > n)
            break;
        r++;
    }

    /* Column index within the row. */
    unsigned __int128 prefix = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)prefix;

    /* Compute T(r,k) = 3^(k+1)*2^(r-k) - 2^(r+1). */
    seq_t term = pow3(k + 1) * pow2(r - k) - pow2(r + 1);
    return term;
}
