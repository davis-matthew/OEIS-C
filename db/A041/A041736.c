/*
 * A041736  Numerators of continued fraction convergents to sqrt(388).
 * https://oeis.org/A041736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A041736
    Numerators of continued fraction convergents to sqrt(388).

    The continued fraction for sqrt(388) is
        [19; 1, 2, 3, 4, 12, 1, 8, 1, 12, 4, 3, 2, 1, 38]
    repeating with period 14 after the initial 19.
    The nth numerator p_n satisfies
        p_0 = 19
        p_1 = 1·p_0 + 1
        p_k = a_k · p_{k-1} + p_{k-2}   (k ≥ 2)
    where a_k is the k‑th partial quotient of the above list.
    The recurrence given by OEIS (a(n)=125619266·a(n-14)-a(n-28))
    follows automatically from the above.

    Initial values:
    a(0)=19, a(1)=20, a(2)=59, a(3)=197, a(4)=847,
    a(5)=10361, a(6)=11208, a(7)=100025, a(8)=111233,
    a(9)=1434821, a(10)=5850517, a(11)=18986372,
    a(12)=43823261, a(13)=62809633, a(14)=2430589315,
    a(15)=2493398948, a(16)=7417387211, a(17)=24745560581,
    a(18)=106399629535, a(19)=1301541115001,
    a(20)=1407940744536, a(21)=12565067071289
*/

#include <stdlib.h>

static const unsigned long long PERIOD[14] = {
    1ULL, 2ULL, 3ULL, 4ULL, 12ULL, 1ULL,
    8ULL, 1ULL, 12ULL, 4ULL, 3ULL, 2ULL,
    1ULL, 38ULL
};
static const size_t PERIOD_LEN = 14;

static seq_t *cache = NULL;      /* array of computed numerators */
static idx_t cache_size = 0;     /* number of terms currently stored */
static size_t cache_cap = 0;     /* allocated capacity of cache */

/* Ensure that cache contains terms 0 … n inclusive */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)19;           /* p0 */
        cache_size = 1;
    }

    while (cache_size <= n) {
        if (cache_size >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache) exit(1);
        }

        idx_t i = cache_size;                      /* compute p_i */
        unsigned long long a_i = (i == 0) ? 19ULL
                            : PERIOD[(i - 1) % PERIOD_LEN];

        seq_t p_i;
        if (i == 1) {
            p_i = (seq_t)a_i * cache[0] + (seq_t)1;
        } else {  /* i >= 2 */
            p_i = (seq_t)a_i * cache[i - 1] + cache[i - 2];
        }

        cache[cache_size] = p_i;
        cache_size++;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
