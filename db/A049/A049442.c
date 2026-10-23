/*
 * A049442  Numbers n such that sum of first n consecutive prime numbers is
 *          pandigital (includes all 10 digits exactly once).
 * https://oeis.org/A049442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   build_primes_upto, digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th integer m for which the sum of the first m
   consecutive prime numbers is a 10‑digit pandigital (contains each of 0–9
   exactly once).  The function builds the list of such indices once and
   looks them up on subsequent calls.  It is written to compile as C11
   and as C++17. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    /* build the list of suitable indices only on the first call */
    static bool init = false;
    static idx_t *indices = NULL;   /* array of indices */
    static size_t idx_cnt = 0;      /* number of indices found */
    static size_t idx_cap = 0;      /* current capacity of the array */

    if (!init) {
        init = true;

        /* initial capacity for the result array */
        idx_cap = 64;
        indices = (idx_t *)malloc(idx_cap * sizeof(idx_t));

        /* pre‑allocate a reasonably large pool of primes so that the
           sum of the first primes never exceeds the largest 10‑digit
           pandigital 9876543210.  2000000 is far larger than needed
           (the 100000‑th prime is < 1.5e6). */
        const idx_t prime_limit = 2000000U;
        uint64_t *pr = NULL;
        size_t prime_cnt = 0;
        build_primes_upto(prime_limit, &pr, &prime_cnt);

        const seq_t max_sum = (seq_t)9876543210ULL;
        seq_t sum = 0;

        for (idx_t i = 1; i <= prime_cnt; ++i) {
            sum += (seq_t)pr[i - 1];
            if (sum > max_sum)
                break;          /* all larger sums are too big */

            unsigned char cnt[10] = {0};
            digit_counts(sum, cnt);

            bool pand = true;
            for (int d = 0; d < 10; ++d) {
                if (cnt[d] != 1) {
                    pand = false;
                    break;
                }
            }
            if (pand) {
                if (idx_cnt >= idx_cap) {
                    idx_cap <<= 1;
                    indices = (idx_t *)realloc(indices, idx_cap * sizeof(idx_t));
                }
                indices[idx_cnt++] = i;   /* store the index */
            }
        }

        free(pr);
    }

    /* return the n‑th term (1‑based).  For n beyond the number of found
       terms or for n == 0, return 0 (undefined behaviour per OEIS
       requirements). */
    if (n == 0 || n > (idx_t)idx_cnt)
        return (seq_t)0;
    return (seq_t)indices[n - 1];
}
