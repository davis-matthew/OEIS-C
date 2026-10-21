/*
 * A229295  Number of solutions to x^2 + y^2 + z^2 == n (mod 2n) for x,y,z in
 *          [0, 2*n).
 * https://oeis.org/A229295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * product over odd primes p | n of C(p^e) * where
 *            C(p^e) is the number of triples (x,y,z) modulo p^e with * x^2 +
 *            y^2 + z^2 ≡ 0 (mod p^e). For e=1, C(p)=p^2. For 2^e: * if e even
 *            (e=2k): C(2^e)=2^(3k) * if e odd (e=2k+1): C(2^e)=6*2^(3k) * See
 *            OEIS A229295 commentary for proof
 * Range:     u8 n <= 8, u16 n <= 105, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0)
        return 0;   /* undefined but safety */

    seq_t result = 1;          /* will accumulate product of local factors */
    idx_t rem   = n;           /* remaining part to factorize */

    /* handle factor 2 separately */
    idx_t e2 = 0;
    while ((rem & 1) == 0) {   /* rem even */
        rem >>= 1;
        e2++;
    }

    if (e2 > 0) {
        seq_t g2;
        if ((e2 & 1) == 0) {            /* e2 even : e2 = 2k */
            idx_t k = e2 >> 1;
            /* 2^(3k) */
            g2 = 1ULL << (3 * k);
        } else {                        /* e2 odd : e2 = 2k+1 */
            idx_t k = (e2 - 1) >> 1;
            g2 = 6ULL * (1ULL << (3 * k));
        }
        result *= g2;
    }

    /* factor the odd part */
    for (idx_t p = 3; p * p <= rem; p += 2) {
        if (rem % p == 0) {
            idx_t exp = 0;
            while (rem % p == 0) {
                rem /= p;
                exp++;
            }

            seq_t gp;
            if (exp == 1) {          /* prime factor to the first power */
                gp = (seq_t)p * (seq_t)p;   /* p^2 */
            } else {
                /* compute p^exp */
                idx_t m = 1;
                for (idx_t i = 0; i < exp; ++i)
                    m *= p;

                /* frequency of squares modulo m */
                int *freq = (int *)calloc((size_t)m, sizeof(int));
                for (idx_t x = 0; x < m; ++x) {
                    idx_t r = (x * x) % m;
                    freq[r]++;
                }

                seq_t total = 0;
                for (idx_t r1 = 0; r1 < m; ++r1) {
                    int f1 = freq[r1];
                    if (!f1)
                        continue;
                    for (idx_t r2 = 0; r2 < m; ++r2) {
                        int f2 = freq[r2];
                        if (!f2)
                            continue;
                        idx_t sum = r1 + r2;
                        if (sum >= m)
                            sum -= m;
                        idx_t r3 = (m - sum) % m;
                        int f3 = freq[r3];
                        if (!f3)
                            continue;
                        total += (seq_t)f1 * (seq_t)f2 * (seq_t)f3;
                    }
                }

                free(freq);
                gp = total;
            }

            result *= gp;
        }
    }

    if (rem > 1) {          /* remaining prime factor > sqrt(original n) */
        idx_t p = rem;
        seq_t gp = (seq_t)p * (seq_t)p;
        result *= gp;
    }

    /* include the parity factor 4 */
    result *= 4ULL;
    return result;
}
