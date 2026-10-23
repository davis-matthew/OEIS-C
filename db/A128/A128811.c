/*
 * A128811  A113551(n)/A006882(n).
 * https://oeis.org/A128811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^m * (3m-1)! / ((m-1)! * m!) for n = 2m (even), a(n) =
 *            (6m+2)! * (m+1)! * m! / (2^m * (3m+1)! * (2m+2)! * (2m)!) for n
 *            = 2m+1 (odd)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   add_factorial_exponents
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/add_factorial_exponents.h"

seq_t a(idx_t n)
{
    /* static data for prime generation */
    static uint32_t *prime_list = NULL;
    static size_t prime_cnt = 0;
    static size_t idx_two = 0;          /* index of prime 2 in the list */
    const uint32_t MAX_PRIME_LIMIT = 30000; /* sufficient for n <= 10000 */

    if (!prime_list) {
        unsigned char *composite = (unsigned char *)calloc(MAX_PRIME_LIMIT + 1, 1);
        prime_list = (uint32_t *)malloc(4000 * sizeof(uint32_t));
        prime_cnt = 0;
        for (uint32_t i = 2; i <= MAX_PRIME_LIMIT; ++i) {
            if (!composite[i]) {
                prime_list[prime_cnt++] = i;
                if ((uint64_t)i * i <= MAX_PRIME_LIMIT) {
                    for (uint32_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                        composite[j] = 1;
                }
            }
        }
        free(composite);
        idx_two = 0; /* prime 2 is the first prime */
    }

    /* exponent array for all primes */
    int *exps = (int *)calloc(prime_cnt, sizeof(int));

    if (n & 1U) {                    /* odd case: n = 2m + 1 */
        idx_t m = (n - 1) >> 1;
        exps[idx_two] -= (int)m;             /* subtract 2^m          */
        add_factorial_exponents(6 * m + 2, +1, exps, prime_list, prime_cnt); /* (6m+2)!   */
        add_factorial_exponents(m + 1, +1, exps, prime_list, prime_cnt);     /* (m+1)!    */
        add_factorial_exponents(m, +1, exps, prime_list, prime_cnt);         /* m!        */
        add_factorial_exponents(3 * m + 1, -1, exps, prime_list, prime_cnt);/* (3m+1)!   */
        add_factorial_exponents(2 * m + 2, -1, exps, prime_list, prime_cnt);/* (2m+2)!   */
        add_factorial_exponents(2 * m,     -1, exps, prime_list, prime_cnt);/* (2m)!     */
    } else {                          /* even case: n = 2m       */
        idx_t m = n >> 1;
        exps[idx_two] += (int)m;             /* add 2^m                */
        add_factorial_exponents(3 * m - 1, +1, exps, prime_list, prime_cnt); /* (3m-1)!   */
        if (m > 0)
            add_factorial_exponents(m - 1, -1, exps, prime_list, prime_cnt);/* (m-1)!   */
        add_factorial_exponents(m, -1, exps, prime_list, prime_cnt);       /* m!        */
    }

    /* build the result from prime powers */
    seq_t result = 1;
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        int e = exps[pi];
        if (e <= 0) continue;
        seq_t power = 1;
        seq_t base  = (seq_t)prime_list[pi];
        idx_t exp   = (idx_t)e;
        while (exp) {
            if (exp & 1) power *= base;
            base *= base;
            exp >>= 1;
        }
        result *= power;
    }

    free(exps);
    return result;
}
