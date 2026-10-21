/*
 * A078906  Expansion of j in powers of Gamma(5)-modular function Lambda^5.
 * https://oeis.org/A078906
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= -1, u16 n <= 0, u32 n <= 3, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A078906:  a(n) = coefficient of x^n in
 * (1+228x+494x^2-228x^3+x^4)^3 / (x(1-11x-x^2)^5).
 * The function uses the recurrence obtained from
 * (x(1-11x-x^2)^5)*G(x) = (1+228x+494x^2-228x^3+x^4)^3
 * where G(x)=Σ a(n)x^n. For n ≥ 13
 *   a(n) = R[n] - Σ_{i=1}^{10} D[i] * a(n-i-1)
 * with D[0..10] = coefficients of (1-11x-x^2)^5
 * and R[0..12] = coefficients of (1+228x+494x^2-228x^3+x^4)^3.           */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

/* Coefficients of (1-11x-x^2)^5   (degree 10) */
static seq_t D[11];
/* Coefficients of (1+228x+494x^2-228x^3+x^4)^3 (degree 12) */
static seq_t R[13];

/* First 14 terms a(-1) … a(12) in string form */
static const char *initial_str[14] = {
    "1",          /* a(-1) */
    "739",        /* a(0)  */
    "196874",     /* a(1)  */
    "22478125",   /* a(2)  */
    "1086128125", /* a(3)  */
    "35307387500",/* a(4)  */
    "913727546875",/* a(5) */
    "20389341653125",/* a(6) */
    "410010534950000",/* a(7) */
    "7633186177665625",/* a(8) */
    "133911227595521875",/* a(9) */
    "2240979684247156250",/* a(10) */
    "36090410657726350000",/* a(11) */
    "563019001047724506250"/* a(12) */
};

/* Cache of computed a(n), index n is shifted by +1 (a(-1) -> idx 0) */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Ensure cache has capacity for at least 'need' entries */
static void ensure_capacity(size_t need)
{
    if (need <= cache_len)
        return;
    size_t new_cap = (cache_len == 0) ? 32 : cache_len;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) /* allocation failed */
        exit(EXIT_FAILURE);
    cache = new_cache;
    cache_len = need;
    /* Zero unused entries */
    for (size_t i = cache_len; i < new_cap; ++i)
        cache[i] = 0;
}

/* Compute D and R once */
static bool coeffs_computed = false;
static void compute_coeffs(void)
{
    if (coeffs_computed)
        return;
    coeffs_computed = true;

    /* Compute D: coefficients of (1-11x-x^2)^5 */
    long long U[3] = {1, -11, -1};
    long long polyU[11] = {0};
    polyU[0] = 1; /* start with 1 */
    size_t degU = 0;
    for (int p = 0; p < 5; ++p) {
        long long tmp[11] = {0};
        for (size_t i = 0; i <= degU; ++i)
            for (size_t j = 0; j < 3; ++j)
                if (i + j < 11)
                    tmp[i + j] += polyU[i] * U[j];
        for (size_t i = 0; i < 11; ++i)
            polyU[i] = tmp[i];
        degU += 2;
    }
    for (size_t i = 0; i < 11; ++i)
        D[i] = (seq_t)polyU[i];

    /* Compute R: coefficients of (1+228x+494x^2-228x^3+x^4)^3 */
    long long P[5] = {1, 228, 494, -228, 1};
    long long polyP[13] = {0};
    polyP[0] = 1;
    size_t degP = 0;
    for (int p = 0; p < 3; ++p) {
        long long tmp[13] = {0};
        for (size_t i = 0; i <= degP; ++i)
            for (size_t j = 0; j < 5; ++j)
                if (i + j < 13)
                    tmp[i + j] += polyP[i] * P[j];
        for (size_t i = 0; i < 13; ++i)
            polyP[i] = tmp[i];
        degP += 4;
    }
    for (size_t i = 0; i < 13; ++i)
        R[i] = (seq_t)polyP[i];

    /* Fill cache with known initial terms */
    compute_coeffs(); /* recursion guard? not needed but safe */
    ensure_capacity(14 + 1); /* indices -1..12 -> size 14+1? */
    /* shift by +1 */
    for (int i = -1; i <= 12; ++i) {
        size_t idx = (size_t)(i + 1);
        cache[idx] = seq_from_str(initial_str[i + 1]); /* i+1 offset for array */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < -1) /* below offset: undefined, return 0 */
        return 0;
    compute_coeffs();

    /* Needed capacity: indices -1 .. n (shift +1) => size n+2 */
    size_t need = (size_t)(n + 2);
    ensure_capacity(need);

    if ((size_t)(n + 1) < cache_len && cache[n + 1] != 0)
        return cache[n + 1];

    /* Compute from first missing index up to n */
    /* Determine start: if cache already filled up to some index */
    size_t start_idx = 0;
    for (size_t i = 0; i < cache_len; ++i)
        if (cache[i] != 0)
            start_idx = i;
    /* start_idx now points to last known cached index */
    if (start_idx > 0)
        start_idx -= 1; /* next one to compute */

    /* Compute sequentially */
    for (size_t k = start_idx + 1; k <= (size_t)n; ++k) {
        /* k corresponds to index of a(k) (since a(-1) at idx 0) */
        seq_t sum = 0;
        for (int i = 1; i <= 10; ++i) {
            seq_t term = cache[(size_t)(k - i)]; /* g_{k-i} corresponds to a(k-i) */
            term *= (seq_t)D[i];
            sum += term;
        }
        seq_t val = 0;
        if ((size_t)k < 13) /* R[k] exists for k <=12 */
            val = R[k];
        /* a(k) = R[k] - sum */
        val -= sum;
        cache[(size_t)(k + 1)] = val; /* shift +1 for storage */
    }

    return cache[(size_t)(n + 1)];
}
