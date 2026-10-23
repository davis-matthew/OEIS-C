/*
 * A042024  Numerators of continued fraction convergents to sqrt(536).
 * https://oeis.org/A042024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042024: Numerators of continued fraction convergents to sqrt(536)
   Formula: a(n) = 291850*a(n-10) - a(n-20)
   Initial values: a(0)=23, a(1)=139, a(2)=162, a(3)=301, a(4)=764,
                    a(5)=4121, a(6)=9006, a(7)=13127, a(8)=22133,
                    a(9)=145925, a(10)=6734683, a(11)=40554023,
                    a(12)=47288706, a(13)=87842729, a(14)=222974164,
                    a(15)=1202713549, a(16)=2628401262, a(17)=3831114811,
                    a(18)=6459516073, a(19)=42588211249 */
#include <stdlib.h>
#include <stdint.h>

#define PRE_LEN 20
static const unsigned long long INIT[PRE_LEN] = {
    23ULL, 139ULL, 162ULL, 301ULL, 764ULL,
    4121ULL, 9006ULL, 13127ULL, 22133ULL, 145925ULL,
    6734683ULL, 40554023ULL, 47288706ULL, 87842729ULL,
    222974164ULL, 1202713549ULL, 2628401262ULL, 3831114811ULL,
    6459516073ULL, 42588211249ULL
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;  /* allocated size of `cache`   */
    static idx_t  done  = 0;  /* largest index computed so far */

    /* initialise cache once */
    if (cache == NULL) {
        cap = 64;
        cache = (seq_t*)malloc((size_t)cap * sizeof(seq_t));
        idx_t i;
        for (i = 0; i < cap; ++i) {
            if (i < PRE_LEN)
                cache[i] = (seq_t)INIT[i];
            else
                cache[i] = 0;
        }
        done = PRE_LEN - 1;
    }

    /* return early if the requested index is within the pre‑filled part */
    if (n < PRE_LEN)
        return cache[n];

    /* grow cache to accommodate index n */
    if (n >= cap) {
        idx_t new_cap = cap;
        while (n >= new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)malloc((size_t)new_cap * sizeof(seq_t));
        idx_t i;
        for (i = 0; i < cap; ++i)
            tmp[i] = cache[i];
        for (i = cap; i < new_cap; ++i)
            tmp[i] = 0;
        free(cache);
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms using the recurrence */
    if (done < n) {
        const seq_t coef = (seq_t)291850ULL;
        for (idx_t i = done + 1; i <= n; ++i) {
            cache[i] = coef * cache[i - 10] - cache[i - 20];
        }
        done = n;
    }

    return cache[n];
}
