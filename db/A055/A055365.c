/*
 * A055365  Number of asymmetric mobiles (circular rooted trees) with n nodes
 *          and 4 leaves.
 * https://oeis.org/A055365
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+13) = 30 - a(n) + 2*a(n+1) + a(n+2) - 3*a(n+3) - a(n+5) +
 *            2*a(n+6) + 2*a(n+7) - a(n+8) - 3*a(n+10) + a(n+11) + 2*a(n+12)
 *            for all n >= 6 Initial values: a(6)=1, a(7)=5, a(8)=19, a(9)=53,
 *            a(10)=130, a(11)=280, a(12)=556, a(13)=1024, a(14)=1788,
 *            a(15)=2971, a(16)=4752, a(17)=7338, a(18)=11013
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 129, u64 n <= 129,
 *            u128 n <= 129, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t capacity = 0;   /* allocated size */
    static idx_t size = 0;      /* first index not yet computed + 1 */

    if (n < 6) return (seq_t)0;

    /* Ensure the array is large enough */
    if (n + 1 > capacity) {
        idx_t new_cap = capacity ? capacity << 1 : 64;
        while (new_cap <= n + 1) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* Initialise base values once */
    if (size == 0) {
        cache[6]  = (seq_t)1;
        cache[7]  = (seq_t)5;
        cache[8]  = (seq_t)19;
        cache[9]  = (seq_t)53;
        cache[10] = (seq_t)130;
        cache[11] = (seq_t)280;
        cache[12] = (seq_t)556;
        cache[13] = (seq_t)1024;
        cache[14] = (seq_t)1788;
        cache[15] = (seq_t)2971;
        cache[16] = (seq_t)4752;
        cache[17] = (seq_t)7338;
        cache[18] = (seq_t)11013;
        size = 19;                  /* next index to compute is 19 */
    }

    /* Compute values up to the requested index */
    if (n + 1 > size) {
        for (idx_t k = size; k <= n; ++k) {
            __int128 rhs = 30;
            rhs -= (__int128)cache[k - 13];
            rhs += 2 * (__int128)cache[k - 12];
            rhs += (__int128)cache[k - 11];
            rhs -= 3 * (__int128)cache[k - 10];
            rhs -= (__int128)cache[k - 8];
            rhs += 2 * (__int128)cache[k - 7];
            rhs += 2 * (__int128)cache[k - 6];
            rhs -= (__int128)cache[k - 5];
            rhs -= 3 * (__int128)cache[k - 3];
            rhs += (__int128)cache[k - 2];
            rhs += 2 * (__int128)cache[k - 1];
            cache[k] = (seq_t)rhs;
        }
        size = n + 1;
    }

    return cache[n];
}
