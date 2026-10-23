/*
 * A121126  Unbranched a-4-catapolynonagons (see Brunvoll reference for
 *          precise definition).
 * https://oeis.org/A121126
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 24*a(n-1) - 204*a(n-2) + 576*a(n-3) + 1260*a(n-4)
 *                - 9504*a(n-5) + 7776*a(n-6) + 31104*a(n-7) - 46656*a(n-8)
 * for n > 13; initial values a(3)=1, a(4)=4, a(5)=40, a(6)=330,
 * a(7)=2814, a(8)=22464, a(9)=174798, a(10)=1321920,
 * a(11)=9798840, a(12)=71383680, a(13)=512709912. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;        /* dynamic cache of a(n) */
static idx_t cache_cap = 0;        /* number of allocated entries */
static idx_t last_computed = 0;    /* highest n for which cache[n] is valid */

static inline void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    idx_t new_cap = n + 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    cache = new_arr;
    cache_cap = new_cap;
}

/* multiplication by a small unsigned integer without overflow
 * for fixed‑width types it uses 128‑bit intermediate values,
 * for arbitrary‑precision types it uses the class operators. */
#ifdef __cplusplus
template <typename T>
static inline T mul_small(const T& a, unsigned int b)
{
    T res = 0;
    T cur = a;
    unsigned int k = b;
    while (k)
    {
        if (k & 1) res += cur;
        if (k > 1) cur += cur;   /* multiplication by 2 */
        k >>= 1;
    }
    return res;
}
#else
static inline seq_t mul_small(seq_t a, unsigned int b)
{
    seq_t res = 0;
    seq_t cur = a;
    unsigned int k = b;
    while (k)
    {
        if (k & 1) res = (seq_t)(((__uint128_t)res + cur));            /* res += cur */
        if (k > 1) cur = (seq_t)(((__uint128_t)cur * 2));               /* cur *= 2 */
        k >>= 1;
    }
    return res;
}
#endif

static void init_cache(void)
{
    if (cache_cap > 0) return;          /* already initialized */
    ensure_cache(13);                   /* need space for indices 0..13   */
    static const seq_t init_vals[11] = {
        (seq_t)1,    (seq_t)4,    (seq_t)40,    (seq_t)330,
        (seq_t)2814, (seq_t)22464, (seq_t)174798, (seq_t)1321920,
        (seq_t)9798840, (seq_t)71383680, (seq_t)512709912
    };
    for (idx_t i = 0; i < 11; ++i)
        cache[3 + i] = init_vals[i];
    last_computed = 13;
}

static void compute_up_to(idx_t n)
{
    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        seq_t val = mul_small(cache[i-1], 24);
        val -= mul_small(cache[i-2], 204);
        val += mul_small(cache[i-3], 576);
        val += mul_small(cache[i-4], 1260);
        val -= mul_small(cache[i-5], 9504);
        val += mul_small(cache[i-6], 7776);
        val += mul_small(cache[i-7], 31104);
        val -= mul_small(cache[i-8], 46656);
        cache[i] = val;
    }
    last_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;               /* values below offset are undefined */
    init_cache();
    if (n <= last_computed) return cache[n];
    ensure_cache(n);
    compute_up_to(n);
    return cache[n];
}
