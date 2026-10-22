/*
 * A006458  Number of elements in Z[ omega ] whose 'smallest algorithm' is <=
 *          n, where omega^2 = -omega - 1.
 * https://oeis.org/A006458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2) - 5*a(n-3) + 4*a(n-4) + 8*a(n-5) -
 *            6*a(n-6) Initial values: a(0) = 1, a(1) = 7, a(2) = 31, a(3) =
 *            115, a(4) = 391, a(5) = 1267
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t capacity = 0;
    static idx_t computed = 0;
    
    static const seq_t init[] = {1, 7, 31, 115, 391, 1267};
    
    if (n < 6) {
        return init[n];
    }
    
    if (!cache) {
        capacity = (n < 512) ? 1024 : (n * 2);
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        for (int i = 0; i < 6; i++) {
            cache[i] = init[i];
        }
        computed = 5;
    }
    
    while (n >= capacity) {
        capacity *= 2;
        cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
    }
    
    for (idx_t i = computed + 1; i <= n; i++) {
        cache[i] = 5*cache[i-1] - 5*cache[i-2] - 5*cache[i-3]
                 + 4*cache[i-4] + 8*cache[i-5] - 6*cache[i-6];
    }
    computed = n;
    
    return cache[n];
}
