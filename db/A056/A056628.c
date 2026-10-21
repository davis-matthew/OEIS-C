/*
 * A056628  a(n) = A056623(n!).
 * https://oeis.org/A056628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A056623(n!) = (∏_{p≤n} p^{⌊v_p(n!)/2⌋ –
 *            (v_p(n!)&1?1:0)})^2
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 17, u64 n <= 25, u128 n <= 27,
 *            big any n
 * Helpers:   build_primes_upto, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/build_primes_upto.h"
#include "../utils/seq_pow.h"
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1 – n should never be <1 */

    /* static cache of primes up to the current maximum n */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t max_n = 0;

    if (n > max_n) {               /* enlarge prime list when needed */
        build_primes_upto(n, &primes, &prime_cnt);
        max_n = n;
    }

    seq_t sqrt_val = (seq_t)1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > n) break;

        /* exponent of p in n! */
        idx_t e = 0;
        idx_t m = n;
        while (m) {
            m /= p;
            e += m;
        }

        idx_t t = e >> 1;           /* floor(e/2) */
        if ((e & 1) && t > 0) {     /* subtract 1 if e is odd and t>0 */
            t--;
        }
        if (t == 0) continue;

        seq_t pow_val = seq_pow((seq_t)p, t);
        sqrt_val = sqrt_val * pow_val;
    }

    return sqrt_val * sqrt_val;   /* a(n) = (sqrt_val)^2 */
}
