/*
 * A106086  Primes p such that 7*p + 2 and 2*p + 7 are primes.
 * https://oeis.org/A106086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 354, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 7619 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: incremental search for primes p where 7p+2 and 2p+7 are primes */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *list = NULL;
    static idx_t cap   = 0;
    static idx_t size  = 0;
    static uint64_t cur = 3;   /* start with the first odd prime candidate */

    while (size < n) {
        /* enlarge storage if necessary */
        if (size + 1 > cap) {
            idx_t newcap = (cap == 0) ? 32 : cap * 2;
            seq_t *tmp = mem_resize(seq_t, list, newcap);
            list = tmp;
            cap  = newcap;
        }

        /* find the next acceptable prime */
        for (;;) {
            if (is_prime_u64(cur)) {
                unsigned __int128 t = (unsigned __int128)cur;
                uint64_t a1 = (uint64_t)(t * 7 + 2);
                uint64_t a2 = (uint64_t)(t * 2 + 7);
                if (is_prime_u64(a1) && is_prime_u64(a2)) {
                    list[size++] = (seq_t)cur;
                    cur += 2;   /* move to the next odd number */
                    break;
                }
            }
            cur += 2; /* skip even numbers */
        }
    }

    return list[n - 1];
}
