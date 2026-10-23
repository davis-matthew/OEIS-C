/*
 * A165527  Number of permutations of length n which avoid the patterns 4231
 *          and 2143.
 * https://oeis.org/A165527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is given by the recurrence for n ≥ 9: a(n) = 13 a(n‑1) – 71
 *            a(n‑2) + 213 a(n‑3) – 386 a(n‑4) + 438 a(n‑5) – 311 a(n‑6) + 133
 *            a(n‑7) – 31 a(n‑8) + 3 a(n‑9) Initial values: a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=6, a(4)=22, a(5)=86, a(6)=335, a(7)=1271, a(8)=4680
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of computed terms */
static seq_t *cache = NULL;          /* pointer to the cache array          */
static size_t cache_len = 0;         /* allocated length (must be >= n+1)   */
static idx_t  last_computed = (idx_t)-1;   /* largest n with a(n) stored */

/* initialise cache with the first 9 terms */
static void init_state(void)
{
    if (cache != NULL) return;

    cache_len = 9;
    cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
    if (!cache) abort();

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)6;
    cache[4] = (seq_t)22;
    cache[5] = (seq_t)86;
    cache[6] = (seq_t)335;
    cache[7] = (seq_t)1271;
    cache[8] = (seq_t)4680;

    last_computed = 8;
}

/* enlarge cache so that it contains indices 0..n */
static void ensure_capacity(idx_t n)
{
    if (n < cache_len) return;
    size_t new_len = cache_len;
    while (new_len <= (size_t)n) new_len <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_len = new_len;
}

/* public function implementing the recurrence */
seq_t a(idx_t n)
{
    init_state();

    if (n <= last_computed) return cache[n];

    ensure_capacity(n);

    /* coefficients of the recurrence */
    static const int coeffs[9] =
        {13, -71, 213, -386, 438, -311, 133, -31, 3};

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (int k = 0; k < 9; ++k) {
            idx_t prev = i - (idx_t)(k + 1);
            int c = coeffs[k];
            seq_t prod = cache[prev] * (seq_t)(c > 0 ? c : -c);
            if (c > 0)
                sum_pos += prod;
            else
                sum_neg += prod;
        }
        cache[i] = sum_pos - sum_neg;  /* coefficients are chosen so the result is positive */
    }
    last_computed = n;
    return cache[n];
}
