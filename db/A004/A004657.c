/*
 * A004657  Expansion of g.f.: (1+x^3)*(1+x^4)/((1-x)*(1-x^2)^2*(1-x^4)).
 * https://oeis.org/A004657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 115, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) - 2*a(n-6) + 2*a(n-7) + a(n-8) - a(n-9) + c(n),
   where c(n) = 1 if n in {0,3,4,7}, 0 otherwise.
   Initial values: a(0)=1, a(1)=1, a(2)=3, a(3)=4, a(4)=9, a(5)=11, a(6)=19, a(7)=24, a(8)=37 */

#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t capacity = 0;
    static idx_t computed_up_to = (idx_t)-1;

    if (cache == NULL) {
        capacity = 256;
        cache = alloc_zero(capacity);
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 3;
        cache[3] = 4;
        cache[4] = 9;
        cache[5] = 11;
        cache[6] = 19;
        cache[7] = 24;
        cache[8] = 37;
        computed_up_to = 8;
    }

    if (n <= computed_up_to) {
        return cache[n];
    }

    if (n >= capacity) {
        idx_t newcap = (n + 1) * 2;
        cache = grow_zero(cache, computed_up_to + 1, newcap);
        capacity = newcap;
    }

    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        seq_t c = ((i == 0) || (i == 3) || (i == 4) || (i == 7)) ? 1 : 0;
        seq_t result = cache[i - 1] + 2 * cache[i - 2] - 2 * cache[i - 3];
        if (i >= 6) result -= 2 * cache[i - 6];
        if (i >= 7) result += 2 * cache[i - 7];
        if (i >= 8) result += cache[i - 8];
        if (i >= 9) result -= cache[i - 9];
        result += c;
        cache[i] = result;
    }
    
    computed_up_to = n;
    return cache[n];
}
