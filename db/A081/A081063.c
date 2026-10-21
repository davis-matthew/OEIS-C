/*
 * A081063  Number of numbers <= n that are 3-smooth or prime powers.
 * https://oeis.org/A081063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=A071521(n)+A065515(n)-A000523(n)-A062153(n)+1
 * Range:     u8 n <= 318, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 1U) return (seq_t)0;  /* safety for n==0 */

    seq_t N = (seq_t)n;

    /* Count 3‑smooth numbers ≤ n (including 1) */
    seq_t count3smooth = 0;
    seq_t pow2 = 1;
    while (pow2 <= N) {
        seq_t val = pow2;
        while (val <= N) {
            ++count3smooth;
            if (val > N / 3) break;
            val *= 3;
        }
        if (pow2 > N / 2) break;
        pow2 *= 2;
    }

    /* Count powers of 2 ≤ n (exponents ≥ 1) */
    seq_t pow2_count = 0;
    seq_t v = 2;
    while (v <= N) {
        ++pow2_count;
        if (v > N / 2) break;
        v *= 2;
    }

    /* Count powers of 3 ≤ n (exponents ≥ 1) */
    seq_t pow3_count = 0;
    seq_t w = 3;
    while (w <= N) {
        ++pow3_count;
        if (w > N / 3) break;
        w *= 3;
    }

    /* Count prime powers ≤ n (p prime, k ≥ 1) */
    seq_t pp = 0;
    size_t prime_cnt = 0;
    uint64_t *primes = NULL;
    build_primes_upto(n, &primes, &prime_cnt);

    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t pv = primes[i];
        while (pv <= N) {
            ++pp;
            if (pv > N / primes[i]) break;
            pv *= primes[i];
        }
    }

    free(primes);  /* clean up the array allocated by build_primes_upto */

    /* Combine the counts according to the OEIS formula */
    seq_t result = count3smooth + pp - pow2_count - pow3_count;
    return result;
}
