/*
 * A145584  a(n) = number of numbers removed in step n of Eratosthenes's sieve
 *          for 2^6.
 * https://oeis.org/A145584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of integers removed at step n of Eratosthenes's sieve for 2^6 (limit = 64) */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 1)   /* offset is 1, undefined for n < 1 */
        return (seq_t)0;

    static seq_t tbl[20];   /* storage for removal counts of each prime up to 64 */
    static size_t tbl_len = 0;   /* number of primes (steps) computed */

    if (tbl_len == 0) {
        /* first call – compute the table for 2^6 = 64 */
        const idx_t LIMIT = ((idx_t)1) << 6;   /* 64 */

        size_t prime_cnt;
        uint64_t *primes;
        build_primes_upto(LIMIT, &primes, &prime_cnt);

        /* sieve array: 1 = still unmarked, 0 = marked */
        uint8_t *sieve = (uint8_t*)calloc((size_t)(LIMIT + 1), sizeof(uint8_t));
        for (idx_t i = 2; i <= LIMIT; ++i)
            sieve[i] = 1;

        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            idx_t p = (idx_t)primes[pi];
            seq_t cnt = 0;

            for (idx_t j = 2 * p; j <= LIMIT; j += p) {
                if (sieve[j]) {
                    sieve[j] = 0;
                    ++cnt;
                }
            }
            if (pi < 20)
                tbl[pi] = cnt;
        }
        tbl_len = prime_cnt;

        free(primes);
        free(sieve);
    }

    /* Return the requested step's removal count, 0 if n exceeds the number of steps */
    if ((idx_t)(n - 1) < tbl_len)
        return tbl[n - 1];
    else
        return (seq_t)0;
}
