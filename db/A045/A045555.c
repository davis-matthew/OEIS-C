/*
 * A045555  Numbers whose factorial has '24' as its final two digits before
 *          the trailing zeros.
 * https://oeis.org/A045555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 3290, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Search for numbers whose factorial ends with 24 before trailing zeros */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2mod.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset is 1, so n==0 is undefined */

    /* static state retained across calls */
    static uint64_t      *idx_list = NULL; /* list of found indices (uint64_t) */
    static size_t        idx_len  = 0;      /* number of found indices */
    static size_t        idx_cap  = 0;      /* capacity of idx_list */
    static uint64_t      last_seen = 0;     /* last integer examined */
    static unsigned int  res_mod   = 1;     /* product of factors without 2s and 5s, modulo 100 */
    static unsigned long cnt2 = 0;          /* total number of 2 factors removed */
    static unsigned long cnt5 = 0;          /* total number of 5 factors removed */

    /* generate terms until we have at least n of them */
    while (idx_len < n) {
        ++last_seen;
        uint64_t m = last_seen;

        /* factor out powers of 2 */
        while ((m & 1ULL) == 0ULL) {
            m >>= 1U;
            ++cnt2;
        }
        /* factor out powers of 5 */
        while (m % 5ULL == 0ULL) {
            m /= 5ULL;
            ++cnt5;
        }
        /* multiply the remaining part modulo 100 */
        unsigned int part = (unsigned int)(m % 100ULL);
        res_mod = (unsigned int)((res_mod * part) % 100);

        /* current non‑zero last two digits of last_seen! */
        unsigned long diff = cnt2 - cnt5;            /* non‑negative */
        unsigned int pow2  = pow2mod(diff);
        unsigned int cur   = (unsigned int)((res_mod * pow2) % 100);

        if (cur == 24) {
            /* grow list if necessary */
            if (idx_len >= idx_cap) {
                size_t new_cap = (idx_cap ? idx_cap * 2 : 16);
                uint64_t *tmp = (uint64_t *)realloc(idx_list, new_cap * sizeof(uint64_t));
                if (!tmp) exit(1); /* memory allocation failure */
                idx_list = tmp;
                idx_cap  = new_cap;
            }
            idx_list[idx_len++] = last_seen;
        }
    }

    return (seq_t)idx_list[n - 1];
}
