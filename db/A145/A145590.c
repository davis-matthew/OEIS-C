/*
 * A145590  a(n)=number of numbers removed in each step of Eratosthenes's
 *          sieve for 2^12.
 * https://oeis.org/A145590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn
 * Formula:   a(n) = number of numbers removed by the nth prime ≤ 64 while
 *            sieving the interval [2, 2^12] using Eratosthenes' sieve. The
 *            value is computed by simulating the sieve
 * Range:     u8 none, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* constants for N = 2^12 */
    const idx_t N     = (idx_t)1 << 12;   /* 4096 */
    const idx_t sqrtN = 64;               /* floor(sqrt(N))  */

    /* static cache */
    static seq_t *vals  = NULL;   /* removal counts for each prime */
    static size_t  len   = 0;     /* number of primes used */
    static bool    init  = false;

    if (!init) {
        uint64_t *primes = NULL;
        size_t cnt;

        /* generate primes ≤ sqrtN = 64 */
        build_primes_upto(sqrtN, &primes, &cnt);
        if (cnt == 0) {
            free(primes);
            init = true;          /* nothing to compute */
            return 0;
        }

        len = cnt;
        vals = (seq_t *)malloc(len * sizeof(seq_t));

        /* sieve array: false = unmarked, true = marked */
        unsigned char *marked = (unsigned char *)calloc((size_t)(N + 1), 1);

        /* simulate sieve for each prime */
        for (size_t i = 0; i < cnt; ++i) {
            seq_t p   = (seq_t)primes[i];
            seq_t cnt_removed = 0;

            /* start at p*p as in classical sieve */
            for (idx_t j = (idx_t)p * (idx_t)p; j <= N; j += (idx_t)p) {
                if (!marked[j]) {
                    marked[j] = 1;
                    ++cnt_removed;
                }
            }
            vals[i] = cnt_removed;
        }

        free(primes);
        free(marked);
        init = true;
    }

    if (n == 0 || n > (idx_t)len)
        return 0;          /* out of range gives 0 */

    return vals[(size_t)(n - 1)];
}
