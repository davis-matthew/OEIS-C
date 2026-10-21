/*
 * A082999  a(n) = A046195(n) mod 5.
 * https://oeis.org/A082999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = +a(n-1) – a(n-3) + a(n-4) – a(n-6) * + a(n-7) – a(n-9) +
 *            a(n-10) – a(n-12) + a(n-13), * where a(1)=1, a(2)=1, a(3)=4,
 *            a(4)=1, a(5)=4, a(6)=0, * a(7)=4, a(8)=0, a(9)=4, a(10)=0,
 *            a(11)=4, a(12)=1, * a(13)=4. All computations are performed
 *            modulo 5
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* 1‑based cache of a(n) values          */
static idx_t  seq_cache_max = 0;  /* largest n for which seq_cache[n] is set*/
static size_t seq_cache_cap = 0;  /* number of elements currently allocated */

static void seq_ensure_capacity(idx_t nmax)
{
    if (seq_cache_cap < (size_t)nmax + 1)
    {
        size_t new_cap = (size_t)nmax + 1;
        void *ptr = realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!ptr) abort();                          /* allocation failed */
        seq_cache = (seq_t *)ptr;                   /* explicit cast for C++ */
        seq_cache_cap = new_cap;
    }
}

static void seq_init_values(void)
{
    seq_ensure_capacity(13);
    /* 0 is a dummy placeholder for 0‑based index; real sequence starts at 1 */
    static const seq_t init_vals[14] =
    { 0, 1, 1, 4, 1, 4, 0, 4, 0, 4, 0, 4, 1, 4 };
    for (idx_t i = 1; i <= 13; ++i) seq_cache[i] = init_vals[i];
    seq_cache_max = 13;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset not used, but kept for safety */

    if (seq_cache_max == 0) seq_init_values();

    if (n <= seq_cache_max) return seq_cache[n];

    seq_ensure_capacity(n);

    for (idx_t i = seq_cache_max + 1; i <= n; ++i)
    {
        int val = (int)seq_cache[i-1]
                - (int)seq_cache[i-3]
                + (int)seq_cache[i-4]
                - (int)seq_cache[i-6]
                + (int)seq_cache[i-7]
                - (int)seq_cache[i-9]
                + (int)seq_cache[i-10]
                - (int)seq_cache[i-12]
                + (int)seq_cache[i-13];
        val %= 5;
        if (val < 0) val += 5;
        seq_cache[i] = (seq_t)val;
    }

    seq_cache_max = n;
    return seq_cache[n];
}
