/*
 * A076244  Distinct values arising in A051547, sequence of a(n) = lcm(phi(1),
 *          ..., phi(n)).
 * https://oeis.org/A076244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(phi(1), ..., phi(n)), with only distinct consecutive
 *            values returned (record sequence)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 23, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Euclidean gcd for arbitrary‑precision seq_t */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute Euler's totient for an idx_t value.
   Uses trial division; result fits in seq_t. */
static seq_t phi(idx_t n)
{
    idx_t m = n;
    seq_t res = (seq_t)n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0)
                m /= p;
            res -= res / (seq_t)p;
        }
    }
    if (m > 1)
        res -= res / (seq_t)m;
    return res;
}

/* Ensure that an array of seq_t has at least 'needed' capacity. */
static void ensure_cap(size_t needed, seq_t **arr, size_t *cap)
{
    if (*cap < needed) {
        size_t new_cap = (*cap > 0) ? (*cap << 1) : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Return the n-th distinct LCM of φ(1)…φ(n). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined for n<1 */

    /* Static state for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;   /* number of distinct values stored (>=1) */
    static seq_t current_lcm = 0;  /* lcm of φ(1)…φ(last_i) */
    static idx_t last_i = 0;       /* last i processed */

    if (!cache) {            /* one‑time allocation */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[1] = (seq_t)1;    /* a(1) = 1 */
        cache_len = 1;
        current_lcm = (seq_t)1;
        last_i = 0;
    }

    /* Grow cache until we have at least 'n' distinct terms. */
    while (cache_len < (size_t)n) {
        idx_t i = last_i + 1;
        seq_t phi_i = phi(i);
        seq_t g = gcd_seq_t(current_lcm, phi_i);
        seq_t temp = current_lcm / g;
        seq_t new_lcm = temp * phi_i;
        current_lcm = new_lcm;

        /* Record a new distinct value if it differs from the last one stored. */
        if (new_lcm != cache[cache_len]) {
            ensure_cap(cache_len + 2, &cache, &cache_cap);
            cache_len++;
            cache[cache_len] = new_lcm;
        }
        last_i = i;
    }

    return cache[n];
}
