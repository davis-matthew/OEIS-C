/*
 * A240047  Number of 2Xn 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or zero plus the sum of the elements
 *          above it or zero plus the sum of the elements diagonally to its
 *          northwest or one plus the sum of the elements antidiagonally to
 *          its northeast, modulo 4.
 * https://oeis.org/A240047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 39, u64 n <= 82, u128 n <= 166,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-2)+4*a(n-3)-3*a(n-4)-7*a(n-5)-5*a(n-6)
   +6*a(n-7)+4*a(n-8)+a(n-9)-3*a(n-10)+2*a(n-11)-a(n-12)
   -2*a(n-14)+4*a(n-15)-2*a(n-18),  for n>22.
   Initial values:
   a(1)=2, a(2)=4, a(3)=9, a(4)=20, a(5)=33, a(6)=76,
   a(7)=117, a(8)=232, a(9)=398, a(10)=675, a(11)=1228,
   a(12)=2002, a(13)=3548, a(14)=5935, a(15)=10128,
   a(16)=17287, a(17)=28967, a(18)=49649, a(19)=83130,
   a(20)=141412, a(21)=238567, a(22)=402406. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Ensure the cache has space for at least 'needed' elements. */
static void ensure_capacity(idx_t needed, size_t *cap, seq_t **ptr) {
    if (needed <= *cap) return;
    size_t new_cap = *cap ? *cap : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    *ptr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* below offset: undefined */

    /* Initial values (index 0 unused). */
    static const seq_t init[23] = {
        0ULL,       /* 0 */
        2ULL, 4ULL, 9ULL, 20ULL, 33ULL, 76ULL, 117ULL,
        232ULL, 398ULL, 675ULL, 1228ULL, 2002ULL, 3548ULL,
        5935ULL, 10128ULL, 17287ULL, 28967ULL, 49649ULL,
        83130ULL, 141412ULL, 238567ULL, 402406ULL
    };

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cached_n = 0;          /* largest n computed so far */

    /* Expand cache up to n if necessary. */
    if (n > cached_n) {
        ensure_capacity((size_t)(n + 1), &cache_cap, &cache);
        cache[0] = (seq_t)0;
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            if (i <= 22) {
                cache[i] = init[(size_t)i];
            } else {
                seq_t pos = 0;
                seq_t neg = 0;

                /* Positive contributions */
                pos += cache[i-2] * 3;
                pos += cache[i-3] * 4;
                pos += cache[i-7] * 6;
                pos += cache[i-8] * 4;
                pos += cache[i-9] * 1;
                pos += cache[i-11] * 2;
                pos += cache[i-15] * 4;

                /* Negative contributions */
                neg += cache[i-4] * 3;
                neg += cache[i-5] * 7;
                neg += cache[i-6] * 5;
                neg += cache[i-10] * 3;
                neg += cache[i-12] * 1;
                neg += cache[i-14] * 2;
                neg += cache[i-18] * 2;

                cache[i] = pos - neg;   /* pos >= neg for all n >= 23 */
            }
        }
        cached_n = n;
    }
    return cache[n];
}
