/*
 * A222124  Number of binary arrays indicating the locations of trailing edge
 *          maxima of a random length-n 0..5 array extended with zeros and
 *          convolved with 1,2,1.
 * https://oeis.org/A222124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-5) + a(n-8) + a(n-10) + 2*a(n-12) -
 *            a(n-13); initial values: a(0)=1, a(1)=2, a(2)=3, a(3)=4, a(4)=8,
 *            a(5)=12, a(6)=19, a(7)=31, a(8)=48, a(9)=79, a(10)=126,
 *            a(11)=201, a(12)=323, a(13)=516
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 93, u128 n <= 188,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static const seq_t init_seq[14] = {1, 2, 3, 4, 8, 12, 19, 31, 48, 79, 126, 201, 323, 516};

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t current_max = 0;
    static bool initialized = false;

    size_t need = (size_t)(n + 1);
    if (cache_cap < need) {
        size_t new_cap = need > 16 ? need : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    if (!initialized) {
        for (idx_t i = 0; i <= 13; ++i)
            cache[i] = init_seq[i];
        current_max = 13;
        initialized = true;
    }

    if (n <= current_max)
        return cache[n];

    for (idx_t i = current_max + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 3] + cache[i - 5] + cache[i - 8] + cache[i - 10]
                 + 2 * cache[i - 12] - cache[i - 13];
    }
    current_max = n;
    return cache[n];
}
