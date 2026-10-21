/*
 * A068793  a(n) = Sum_{i=1..n} i*(n^(n-i-1) + n^(n+i-1)).
 * https://oeis.org/A068793
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^n*(n^n*(n-2)+2)-n^2+n-1)/(n-1)^2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow, u128_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/u128_pow.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* offset starts at 2 */

    /* Fast path: seq_t is 64‑bit or smaller → use 128‑bit arithmetic for intermediates. */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned __int128 n128   = n;
        unsigned __int128 n_pow_n = u128_pow(n128, n);               /* n^n        */
        unsigned __int128 inner   = n_pow_n * (n128 - 2) + 2;        /* n^n*(n-2)+2 */
        unsigned __int128 term    = n_pow_n * inner;               /* n^n*inner   */
        unsigned __int128 numer   = term - n128 * n128 + n128 - 1;   /* numerator   */
        unsigned __int128 denom   = (n128 - 1) * (n128 - 1);         /* denominator */
        return (seq_t)(numer / denom);
    }

    /* General path: seq_t is larger (e.g. big‑integer). */
    seq_t nseq   = (seq_t)n;
    seq_t n_pow_n = big_pow(nseq, nseq);                        /* n^n           */
    seq_t inner   = n_pow_n * ((nseq - 2) + 0) + 2;             /* n^n*(n-2)+2   */
    seq_t term    = n_pow_n * inner;                            /* n^n*inner     */
    seq_t numer   = term - nseq * nseq + nseq - 1;              /* numerator     */
    seq_t denom   = (nseq - 1) * (nseq - 1);                    /* denominator   */
    return numer / denom;
}
