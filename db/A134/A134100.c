/*
 * A134100  Primes p > 3 such that neither p-2 nor p-4 are prime.
 * https://oeis.org/A134100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 4821, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   trial_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iterate through increasing primes p > 3 and check if both p-2
 * and p-4 are composite.  Store such primes in a static array and return the
 * n-th element (1‑based).  The function uses trial division with a growing
 * list of known primes (starting with 2 and 3). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/trial_uint64.h"

seq_t a(idx_t n)
{
    /* static state across calls */
    static uint64_t *primes_all = NULL;   /* array of known primes */
    static size_t primes_cap   = 0;
    static size_t primes_cnt   = 0;

    static seq_t *special = NULL;         /* primes satisfying the condition */
    static size_t sp_cap   = 0;
    static size_t sp_cnt   = 0;

    static uint64_t cur = 5;               /* current odd candidate > 3 */

    /* First‑time initialization */
    if (!primes_all) {
        primes_cap = 4;
        primes_all = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes_all[0] = 2;
        primes_all[1] = 3;
        primes_cnt = 2;
    }
    if (!special) {
        sp_cap = 4;
        special = (seq_t *)malloc(sp_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (sp_cnt < (size_t)n) {
        /* Find the next prime */
        while (!trial_uint64(cur, primes_all, primes_cnt))
            cur += 2;                 /* skip composites, stay odd */

        /* Check the definition: neither p-2 nor p-4 are prime */
        bool good = true;
        if (cur > 3) {
            if (trial_uint64(cur - 2, primes_all, primes_cnt) ||
                trial_uint64(cur - 4, primes_all, primes_cnt))
                good = false;
        }

        /* Store the prime if it qualifies */
        if (good) {
            if (sp_cnt == sp_cap) {
                sp_cap = sp_cap * 2;
                special = (seq_t *)realloc(special, sp_cap * sizeof(seq_t));
            }
            special[sp_cnt++] = (seq_t)cur;
        }

        /* Add the current prime to the prime list for future tests */
        if (primes_cnt == primes_cap) {
            primes_cap = primes_cap * 2;
            primes_all = (uint64_t *)realloc(primes_all, primes_cap * sizeof(uint64_t));
        }
        primes_all[primes_cnt++] = cur;

        /* Move to the next odd candidate */
        cur += 2;
    }

    return special[(size_t)n - 1];
}
