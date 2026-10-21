/*
 * A221938  Number of 2 X n arrays of occupancy after each element stays put
 *          or moves to some horizontal or antidiagonal neighbor, without
 *          move-in move-out straight through or left turns.
 * https://oeis.org/A221938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 25*a(n-2) + 40*a(n-3) + a(n-4) - 42*a(n-5) -
 *            20*a(n-6) + 10*a(n-7) + 4*a(n-8) - a(n-9) Initial values:
 *            a(1)=1, a(2)=17, a(3)=105, a(4)=783, a(5)=5622, a(6)=40608,
 *            a(7)=293084, a(8)=2115379, a(9)=15268366 (Offset 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* 1‑indexed array of computed terms */
static size_t cache_cap = 0;     /* allocated capacity (excluding dummy index 0) */
static idx_t  cache_len = 0;     /* highest computed index */

/* Ensure the cache can store at least n terms (indexed 1..n). */
static void ensure_cache_capacity(idx_t n)
{
    if (cache_cap >= (size_t)n)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < (size_t)n)
        new_cap <<= 1;
    seq_t *tmp;
    if (cache == NULL) {
        tmp = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();
    } else {
        tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialize cache with the known first nine terms. */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    ensure_cache_capacity(9);
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)17;
    cache[3] = (seq_t)105;
    cache[4] = (seq_t)783;
    cache[5] = (seq_t)5622;
    cache[6] = (seq_t)40608;
    cache[7] = (seq_t)293084;
    cache[8] = (seq_t)2115379;
    cache[9] = (seq_t)15268366;
    cache_len = 9;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    init_cache();

    if (n <= cache_len)
        return cache[n];

    ensure_cache_capacity(n);

    for (idx_t k = cache_len + 1; k <= n; ++k) {
        seq_t val = 0;
        val += (seq_t)3   * cache[k - 1];
        val += (seq_t)25  * cache[k - 2];
        val += (seq_t)40  * cache[k - 3];
        val +=              cache[k - 4];
        val -= (seq_t)42  * cache[k - 5];
        val -= (seq_t)20  * cache[k - 6];
        val += (seq_t)10  * cache[k - 7];
        val += (seq_t)4   * cache[k - 8];
        val -=              cache[k - 9];
        cache[k] = val;
    }
    cache_len = n;
    return cache[n];
}
