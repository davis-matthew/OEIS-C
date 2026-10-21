/*
 * A224305  Number of n X 3 0..2 arrays with diagonals and rows unimodal and
 *          antidiagonals nondecreasing.
 * https://oeis.org/A224305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A224305 (order 17):
   a(1)=22, a(2)=218, a(3)=1586, a(4)=11361, a(5)=82700,
   a(6)=615481, a(7)=4634768, a(8)=35003328, a(9)=264487714,
   a(10)=1997888432, a(11)=15087449584, a(12)=113921725004,
   a(13)=860155842566, a(14)=6494472790364, a(15)=49035576285772,
   a(16)=370236844834368, a(17)=2795429838028120.
   For n>17:
   a(n)=10*a(n-1)-16*a(n-2)-24*a(n-3)-44*a(n-4)+728*a(n-5)
        -678*a(n-6)-722*a(n-7)-860*a(n-8)+9336*a(n-9)
        -3916*a(n-10)+792*a(n-11)+320*a(n-12)-11936*a(n-13)
        +2848*a(n-14)+128*a(n-15)+384*a(n-16)+3840*a(n-17). */

#include <stdlib.h>

/* Initial terms of the sequence */
static const seq_t init_vals[17] = {
    (seq_t)22ULL,                 /* a(1)  */
    (seq_t)218ULL,                /* a(2)  */
    (seq_t)1586ULL,               /* a(3)  */
    (seq_t)11361ULL,              /* a(4)  */
    (seq_t)82700ULL,              /* a(5)  */
    (seq_t)615481ULL,             /* a(6)  */
    (seq_t)4634768ULL,            /* a(7)  */
    (seq_t)35003328ULL,           /* a(8)  */
    (seq_t)264487714ULL,          /* a(9)  */
    (seq_t)1997888432ULL,         /* a(10) */
    (seq_t)15087449584ULL,        /* a(11) */
    (seq_t)113921725004ULL,       /* a(12) */
    (seq_t)860155842566ULL,       /* a(13) */
    (seq_t)6494472790364ULL,      /* a(14) */
    (seq_t)49035576285772ULL,     /* a(15) */
    (seq_t)370236844834368ULL,    /* a(16) */
    (seq_t)2795429838028120ULL    /* a(17) */
};

/* Coefficients for the recurrence */
static const int coeff[17] = {
    10, -16, -24, -44, 728,
   -678, -722, -860, 9336, -3916,
    792, 320, -11936, 2848, 128,
    384, 3840
};

/* Ensure that the cache has capacity for indices up to max_idx (0‑based) */
static void ensure_cache(seq_t **cache_ptr, size_t *cap_ptr, size_t max_idx)
{
    if (*cap_ptr <= max_idx) {
        size_t new_cap = *cap_ptr ? *cap_ptr * 2 : 16;
        while (new_cap <= max_idx) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(*cache_ptr, new_cap * sizeof(seq_t));
        *cache_ptr = tmp;
        *cap_ptr = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n == 0) return (seq_t)0;       /* Undefined below offset */

    size_t idx = (size_t)n - 1;         /* 0‑based index */

    if (idx < cache_len) return cache[idx];

    ensure_cache(&cache, &cache_cap, idx);

    /* Fill initial values if not yet cached */
    if (cache_len < 17) {
        size_t upto = idx < 16 ? idx + 1 : 17;
        for (size_t i = cache_len; i < upto; ++i)
            cache[i] = init_vals[i];
    }

    /* Compute subsequent terms using the recurrence */
    size_t start = (cache_len > 17) ? cache_len : 17;
    for (size_t i = start; i <= idx; ++i) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        for (int k = 0; k < 17; ++k) {
            seq_t term = cache[i - (k + 1)];
            if (coeff[k] >= 0)
                pos_sum += term * (seq_t)coeff[k];
            else
                neg_sum += term * (seq_t)(-coeff[k]);
        }
        cache[i] = pos_sum - neg_sum;
    }

    if (cache_len <= idx) cache_len = idx + 1;
    return cache[idx];
}
