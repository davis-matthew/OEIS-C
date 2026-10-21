/*
 * A213349  7-quantum transitions in systems of N >= 7 spin 1/2 particles, in
 *          columns by combination indices.
 * https://oeis.org/A213349
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = flattened triangle of T(7;N,k) where T(7;N,k) =
 *            2^(N-7-2k) * binomial(N,k) * binomial(N-k,7+k) with N ≥ 7, k =
 *            0..floor((N-7)/2). a(7) is the first term of the sequence
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 57, u64 n <= 236,
 *            u128 n <= 979, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 7. For n < 7 the value is
       unspecified – return 0 for safety. */
    if (n < 7)
        return (seq_t)0;

    /* Convert the 1‑based index into a 0‑based position in the
       flattened triangle starting at N = 7. */
    idx_t pos = n - 7;          /* 0‑based position in all terms */
    idx_t N   = 7;              /* current row number */

    while (1) {
        idx_t len = ((N - 7) / 2) + 1;   /* number of terms in this row */
        if (pos < len)
            break;                      /* found the correct row */
        pos -= len;                     /* skip this row */
        ++N;
    }

    /* Now we are in row N, and 'pos' is the offset k within that row. */
    idx_t k   = pos;
    idx_t exp = N - 7 - 2 * k;          /* exponent for 2 */

    /* Compute the value using the provided big‑integer utilities. */
    seq_t result = pow2(exp);                    /* 2^exp */
    result = result * binomial(N, k);            /* * C(N,k) */
    result = result * binomial(N - k, 7 + k);    /* * C(N-k,7+k) */

    return result;
}
