/*
 * A032728  Numbers k such that k prefixed by '3' and followed by '9' is
 *          prime.
 * https://oeis.org/A032728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers k such that 3*10^(digits(k)+1)+k*10+9 is prime
 * Range:     u8 none, u16 n <= 302, u32 n <= 248, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digits, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digits.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static idx_t next_k = 1;

static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < need) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, so n=0 is undefined */
    while (cache_size < n) {
        idx_t k = next_k++;
        int d = digits(k);           /* number of decimal digits of k */
        seq_t mult = 1;              /* 10^(d+1) */
        for (int i = 0; i < d + 1; i++) mult *= 10;
        seq_t N = 3 * mult + ((seq_t)k) * 10 + 9;
        if (is_prime_big(N)) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = k;
        }
    }
    return cache[n - 1];
}
