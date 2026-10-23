/*
 * A102117  Iccanobirt numbers (7 of 15): a(n) = R(a(n-1)) + R(a(n-2)) +
 *          R(a(n-3)), where R is the digit reversal function A004086.
 * https://oeis.org/A102117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = R(a(n-1)) + R(a(n-2)) + R(a(n-3)), a(0)=0, a(1)=0, a(2)=1
 * Range:     u8 n <= 13, u16 n <= 17, u32 n <= 28, u64 n <= 62, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Reverse the decimal digits of x. */
static seq_t reverse_digits(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + (x % 10);
        x /= 10;
    }
    return rev;
}

seq_t a(idx_t n)
{
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    if (n < cache.size())
        return cache[(size_t)n];
    size_t start = cache.size();
    cache.resize((size_t)n + 1);
    for (size_t i = start; i <= (size_t)n; ++i) {
        seq_t val;
        if (i < 3) {
            if (i == 0 || i == 1)
                val = 0;
            else /* i == 2 */
                val = 1;
        } else {
            val = reverse_digits(cache[i-1]) + reverse_digits(cache[i-2]) + reverse_digits(cache[i-3]);
        }
        cache[i] = val;
    }
    return cache[(size_t)n];
#else
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    if (n < cache_size)
        return cache[n];
    size_t start = cache_size;
    cache = (seq_t *)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
    if (!cache)
        return 0; /* Allocation failure */
    cache_size = (idx_t)n + 1;
    for (idx_t i = start; i <= n; ++i) {
        seq_t val;
        if (i < 3) {
            if (i == 0 || i == 1)
                val = 0;
            else /* i == 2 */
                val = 1;
        } else {
            val = reverse_digits(cache[i-1]) + reverse_digits(cache[i-2]) + reverse_digits(cache[i-3]);
        }
        cache[i] = val;
    }
    return cache[n];
#endif
}
