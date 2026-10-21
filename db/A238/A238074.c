/*
 * A238074  Number of (n+1)X(1+1) 0..3 arrays with the maximum plus the
 *          minimum of every 2X2 subblock differing from its horizontal and
 *          vertical neighbors by exactly one.
 * https://oeis.org/A238074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 17*a(n-2) + 736*a(n-4) + 2353*a(n-6)
 *            + 2483*a(n-8) + 800*a(n-10) for n > 11 with initial values
 *            a(1)=256, a(2)=1592, a(3)=9884, a(4)=59772, a(5)=378708,
 *            a(6)=2285576, a(7)=14349272, a(8)=86693008, a(9)=546582828,
 *            a(10)=3300593268, a(11)=20768825876, a(12)=125443787248,...
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 – a(0) not used */

    static seq_t *cache = NULL;
    static idx_t capacity = 0;
    static idx_t max_computed = 0;

    /* allocate or grow cache to hold index n */
    if (!cache || n >= capacity) {
        idx_t new_cap = (n < 10000 ? 10001U : n + 1U);
        seq_t *tmp = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;        /* out‑of‑memory guard */

        if (cache) {
            memcpy(tmp, cache, sizeof(seq_t) * capacity);
            free(cache);
        }
        cache = tmp;
        capacity = new_cap;
    }

    /* if value already known, return it */
    if (n <= max_computed) return cache[n];

    /* initial 18 values, labelled from 1 */
    static const seq_t init[19] = {
        /* 0 */ (seq_t)0,
        /* 1 */ (seq_t)256ULL,
        /* 2 */ (seq_t)1592ULL,
        /* 3 */ (seq_t)9884ULL,
        /* 4 */ (seq_t)59772ULL,
        /* 5 */ (seq_t)378708ULL,
        /* 6 */ (seq_t)2285576ULL,
        /* 7 */ (seq_t)14349272ULL,
        /* 8 */ (seq_t)86693008ULL,
        /* 9 */ (seq_t)546582828ULL,
        /*10 */ (seq_t)3300593268ULL,
        /*11 */ (seq_t)20768825876ULL,
        /*12 */ (seq_t)125443787248ULL,
        /*13 */ (seq_t)790067077480ULL,
        /*14 */ (seq_t)4771492579096ULL,
        /*15 */ (seq_t)30039037764956ULL,
        /*16 */ (seq_t)181425384418428ULL,
        /*17 */ (seq_t)1142390702895284ULL,
        /*18 */ (seq_t)6899484032213320ULL
    };

    /* initialise cache with known values up to the requested n, if needed */
    if (max_computed < 18) {
        idx_t up = 18;
        if (n < up) up = n;
        for (idx_t i = max_computed + 1; i <= up; ++i) {
            cache[i] = init[i];
        }
        max_computed = up;
    }

    /* compute subsequent terms using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] =
            (seq_t)17   * cache[i-2] +
            (seq_t)736   * cache[i-4] +
            (seq_t)2353  * cache[i-6] +
            (seq_t)2483  * cache[i-8] +
            (seq_t)800   * cache[i-10];
    }
    max_computed = n;
    return cache[n];
}
