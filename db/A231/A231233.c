/*
 * A231233  Triangle T(n,k) = greatest prime factor of n*k+1.
 * https://oeis.org/A231233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = greatest prime factor of r*k+1 where r is the row number
 *            given by n = (r-1)*r/2 + k (1 ≤ k ≤ r)
 * Range:     u8 n <= 22, u16 n <= 4129, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"

/* Return the n-th term of A231233. */
seq_t a(idx_t n) {
    /* convert n to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;

    /* --- find the row number r: smallest r with r*(r+1)/2 ≥ n --- */
    seq_t low = 1, high = (seq_t)(2 * n_seq);  /* upper bound for r */
    while (low < high) {
        seq_t mid = low + (high - low) / 2;
        seq_t tri = mid * (mid + 1) / 2;
        if (tri < n_seq)
            low = mid + 1;
        else
            high = mid;
    }
    seq_t r = low;

    /* --- column index k --- */
    seq_t k = n_seq - (r - 1) * r / 2;

    /* --- compute m = r*k + 1 --- */
    seq_t m = r * k + 1;

    /* --- factor m to find its greatest prime factor --- */
    /* limit for prime list is sqrt(m) */
    seq_t limit = floor_sqrt(m);
    if (limit < 2) limit = 2;                  /* ensure at least 2 */

    size_t p_cnt;
    uint64_t *primes;
    build_primes_upto((idx_t)limit, &primes, &p_cnt);

    seq_t largest = 1;
    seq_t v = m;

    for (size_t i = 0; i < p_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t p_seq = (seq_t)p;
        seq_t p_sq = p_seq * p_seq;
        if (p_sq > v) break;
        while (v % p_seq == 0) {
            largest = p_seq;
            v /= p_seq;
        }
    }
    if (v > 1) {
        if (v > largest) largest = v;
    }

    free(primes);
    return largest;
}
