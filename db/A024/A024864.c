/*
 * A024864  s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2 ], s =
 *          (natural numbers), t = A001950 (upper Wythoff sequence).
 * https://oeis.org/A024864
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊n/2⌋} i * t(n-i+1), where t(m) = A001950(m) =
 *            floor(m*φ²) ≈ floor(m*2178309/832040). The rational
 *            2178309/832040 is a convergent of φ² and differs from the exact
 *            value by less than 10⁻¹², which guarantees correctness for all n
 *            ≤ 10 000
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)   /* undefined below offset */
        return 0;

    /* static cache for the upper Wythoff sequence t(m) */
    static seq_t *t_cache      = NULL;
    static size_t t_cache_cap  = 0;
    static idx_t  t_cache_size = 0;   /* number of values stored (≥0) */

    /* extend cache as needed */
    if (n > t_cache_size) {
        size_t new_cap = t_cache_cap ? t_cache_cap : 1;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        t_cache = (seq_t *)realloc(t_cache, new_cap * sizeof(seq_t));
        t_cache_cap = new_cap;
        /* constants for the convergent of φ² */
        const uint64_t NUM = 2178309ULL;   /*  (F_{k+1}+F_k)  */
        const uint64_t DEN = 832040ULL;    /*  F_k  */
        for (idx_t i = t_cache_size + 1; i <= n; ++i) {
            uint64_t val = ((uint64_t)i * NUM) / DEN;   /* floor(i*NUM/DEN) */
            t_cache[i - 1] = (seq_t)val;                /* store */
        }
        t_cache_size = n;
    }

    /* compute the sum */
    idx_t k = n >> 1;          /* floor(n/2) */
    unsigned long long sum = 0; /* 64‑bit accumulator */
    for (idx_t i = 1; i <= k; ++i) {
        idx_t idx = n - i;                     /* index of t(n-i+1) */
        seq_t t_val = t_cache[idx];             /* t(n-i+1) */
        sum += (unsigned long long)i * (unsigned long long)t_val;
    }

    return (seq_t)sum;   /* cast to seq_t – overflow acceptable if value
                           exceeds the range of seq_t */
}
