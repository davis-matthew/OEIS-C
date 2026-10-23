/*
 * A032247  "DHK[ 6 ]" (bracelet, identity, unlabeled, 6 parts) transform of
 *          1,1,1,1,...
 * https://oeis.org/A032247
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-3) - 2*a(n-5) + 3*a(n-6) - 2*a(n-7) +
 *            a(n-8) + a(n-9) - 2*a(n-10) + 3*a(n-11) - 2*a(n-12) - a(n-14) +
 *            2*a(n-16) - a(n-17) with initial values for n = 9..25: a(9)=3,
 *            a(10)=6, a(11)=16, a(12)=29, a(13)=56, a(14)=90, a(15)=150,
 *            a(16)=222, a(17)=336, a(18)=474, a(19)=672, a(20)=...
 * Range:     u8 n <= 16, u16 n <= 42, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[17] = {
    (seq_t)3, (seq_t)6, (seq_t)16, (seq_t)29, (seq_t)56, (seq_t)90,
    (seq_t)150, (seq_t)222, (seq_t)336, (seq_t)474, (seq_t)672,
    (seq_t)908, (seq_t)1233, (seq_t)1612, (seq_t)2112, (seq_t)2693,
    (seq_t)3432
};

static seq_t *a_cache = NULL;      /* storage for computed values */
static idx_t cache_cap = 0;        /* current capacity of a_cache */

/* Ensure that a_cache can hold values up to at least `n` */
static void ensure_capacity(idx_t n)
{
    if (a_cache && cache_cap > n) return;
    idx_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *ptr = (seq_t *)realloc(a_cache, (new_cap + 1) * sizeof(seq_t));
    if (!ptr) { /* allocation failure: terminate */
        abort();
    }
    a_cache = ptr;
    if (!cache_cap) { /* first time allocation: zero & init first values */
        for (idx_t i = 0; i <= new_cap; ++i) a_cache[i] = 0;
        for (idx_t i = 9; i <= 25 && i <= new_cap; ++i)
            a_cache[i] = init_vals[i - 9];
    }
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 9) return 0;       /* offset behaviour not required */
    ensure_capacity(n);
    if (a_cache[n]) return a_cache[n];  /* already computed */

    /* compute sequentially up to n */
    idx_t start = 26;          /* values up to 25 are known */
    if (a_cache[25] == 0) {    /* first call, init base values */
        for (idx_t i = 9; i <= 25; ++i)
            a_cache[i] = init_vals[i - 9];
    }
    for (idx_t k = start; k <= n; ++k) {
        seq_t sum_pos = 0, sum_neg = 0;
        sum_pos += 2 * a_cache[k - 1];
        sum_pos += 3 * a_cache[k - 6];
        sum_pos += a_cache[k - 8];
        sum_pos += a_cache[k - 9];
        sum_pos += 3 * a_cache[k - 11];
        sum_pos += 2 * a_cache[k - 16];

        sum_neg += a_cache[k - 3];
        sum_neg += 2 * a_cache[k - 5];
        sum_neg += 2 * a_cache[k - 7];
        sum_neg += 2 * a_cache[k - 10];
        sum_neg += 2 * a_cache[k - 12];
        sum_neg += a_cache[k - 14];
        sum_neg += a_cache[k - 17];

        a_cache[k] = sum_pos - sum_neg;
    }
    return a_cache[n];
}
