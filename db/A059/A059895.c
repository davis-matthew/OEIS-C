/*
 * A059895  Table a(i,j) = product prime[k]^(Ei[k] AND Ej[k]) where Ei and Ej
 *          are the vectors of exponents in the prime factorizations of i and
 *          j; AND is the bitwise operation on binary representation of the
 *          exponents.
 * https://oeis.org/A059895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   a(n) is the value of the A059895 table at position n. The table
 *            entry a(i,j) = ∏ prime_k^(Ei[k] & Ej[k]) where Ei, Ej are the
 *            exponent vectors of i and j
 * Range:     u8 n <= 9288, u16 n <= 9288, u32 n <= 9288, u64 n <= 9288,
 *            u128 n <= 9288, big any n
 * Helpers:   factor_u64, seq_pow, tri_ge
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/tri_ge.h"

seq_t a(idx_t n)
{
    /* Find s such that T(s-1) < n ≤ T(s)  (T(s)=s*(s-1)/2) */
    idx_t low  = 1;
    idx_t high = n * 2;          /* safe upper bound */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (tri_ge(mid, n))
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;

    idx_t prev = (s > 1) ? (( (__int128)(s - 1) * (s - 2) ) / 2) : 0;
    idx_t i = n - prev;          /* row index (1‑based) */
    idx_t j = s - i;             /* column index (1‑based) */

    /* Factor i and j */
    uint64_t primes_i[64], exps_i[64];
    uint64_t primes_j[64], exps_j[64];
    size_t cnt_i = 0, cnt_j = 0;
    factor_u64(i, primes_i, exps_i, &cnt_i);
    factor_u64(j, primes_j, exps_j, &cnt_j);

    seq_t result = 1;
    size_t ii = 0, jj = 0;
    while (ii < cnt_i && jj < cnt_j) {
        if (primes_i[ii] < primes_j[jj]) {
            ++ii;
        } else if (primes_j[jj] < primes_i[ii]) {
            ++jj;
        } else {
            uint64_t p = primes_i[ii];
            uint64_t e = exps_i[ii] & exps_j[jj];   /* bitwise AND of exponents */
            if (e != 0) {
                seq_t pow_val = seq_pow((seq_t)p, (idx_t)e);
                result *= pow_val;
            }
            ++ii;
            ++jj;
        }
    }
    return result;
}
