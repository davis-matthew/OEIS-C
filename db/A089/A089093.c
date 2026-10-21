/*
 * A089093  Convoluted convolved Fibonacci numbers G_j^(6).
 * https://oeis.org/A089093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.:
 *            (z/6)[1/(1-z-z^2)^6-1/(1-z^2-z^4)^3-1/(1-z^3-z^6)^2+1/(1-z^6-z^12)]
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 25, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* The provided build specifies seq_t and idx_t */
extern seq_t T_value(idx_t r, idx_t k);
extern seq_t a_rec(idx_t n);

/*--------------------------------------------------------------------
   Local static data
--------------------------------------------------------------------*/
static seq_t D[49];                 /* denominator coefficients, D[0]=1 */
static seq_t init_a[48];            /* first 48 sequence values (a(1)..a(48)) */
static int   init_done = 0;         /* has the init routine been executed? */

/* sequence cache for arbitrary n, 0‑based: cache[i] = a(i+1) */
static seq_t *seq_cache = NULL;
static size_t cache_len = 0;        /* number of terms stored in cache */

/*--------------------------------------------------------------------
   Utility: polynomial multiplication (truncated to given length)
--------------------------------------------------------------------*/
static void poly_mul(const seq_t *a, size_t a_len,
                     const seq_t *b, size_t b_len,
                     seq_t *res, size_t res_len)
{
    for (size_t i = 0; i < res_len; ++i) res[i] = 0;
    for (size_t i = 0; i < a_len; ++i)
        for (size_t j = 0; j < b_len; ++j)
            if (i + j < res_len)
                res[i + j] += a[i] * b[j];
}

/*--------------------------------------------------------------------
   Initialise denominator, first 48 terms and cache up to a(48)
--------------------------------------------------------------------*/
static void init_sequence(void)
{
    if (init_done) return;
    init_done = 1;

    /* Fibonacci shift f[n] = Fib(n+1),  f[0]=1, f[1]=1 */
    seq_t f[48];
    f[0] = 1; f[1] = 1;
    for (int i = 2; i < 48; ++i) f[i] = f[i-1] + f[i-2];

    /* convolution f^2 (up to 47) */
    seq_t conv2[48] = {0};
    for (int i = 0; i < 48; ++i)
        for (int j = 0; j < 48; ++j)
            if (i + j < 48) conv2[i + j] += f[i] * f[j];

    /* convolution f^3 */
    seq_t conv3[48] = {0};
    for (int i = 0; i < 48; ++i)
        for (int j = 0; j < 48; ++j)
            if (i + j < 48) conv3[i + j] += conv2[i] * f[j];

    /* convolution f^6 */
    seq_t cur[48], nxt[48];
    for (int i = 0; i < 48; ++i) cur[i] = f[i];
    for (int step = 1; step < 6; ++step) {
        for (int i = 0; i < 48; ++i) nxt[i] = 0;
        for (int i = 0; i < 48; ++i)
            for (int j = 0; j < 48; ++j)
                if (i + j < 48) nxt[i + j] += cur[i] * f[j];
        for (int i = 0; i < 48; ++i) cur[i] = nxt[i];
    }
    seq_t conv6[48];
    for (int i = 0; i < 48; ++i) conv6[i] = cur[i];

    /* first 48 terms of the sequence (a(1)..a(48)) */
    for (int n = 0; n < 48; ++n) {
        seq_t c = conv6[n];
        seq_t d = (n % 2 == 0) ? conv3[n/2] : 0;
        seq_t e = (n % 3 == 0) ? conv2[n/3] : 0;
        seq_t g = (n % 6 == 0) ? f[n/6] : 0;
        init_a[n] = (c - d - e + g) / 6;   /* integral division */
    }

    /* Build the denominator polynomial D(z) = (1-z-z^2)^6
       (1-z^2-z^4)^3 (1-z^3-z^6)^2 (1-z^6-z^12)           */
    seq_t poly1[13] = {0}, poly2[13] = {0},
           poly3[13] = {0}, poly4[13] = {0};
    seq_t tmp[13] = {0};

    /* (1 - z - z^2)^6 */
    poly1[0] = 1;
    for (int t = 0; t < 6; ++t) {
        for (int i = 0; i < 13; ++i) tmp[i] = 0;
        for (int i = 0; i < 13; ++i)
            for (int o = 0; o <= 2; ++o) {
                int deg = i + o;
                if (deg >= 13) continue;
                seq_t coeff = 1;
                if (o > 0) coeff = -1;
                tmp[deg] += poly1[i] * coeff;
            }
        for (int i = 0; i < 13; ++i) poly1[i] = tmp[i];
    }

    /* (1 - z^2 - z^4)^3 */
    poly2[0] = 1;
    for (int t = 0; t < 3; ++t) {
        for (int i = 0; i < 13; ++i) tmp[i] = 0;
        for (int i = 0; i < 13; ++i)
            for (int o = 0; o <= 4; o += 2) {
                int deg = i + o;
                if (deg >= 13) continue;
                seq_t coeff = 1;
                if (o > 0) coeff = -1;
                tmp[deg] += poly2[i] * coeff;
            }
        for (int i = 0; i < 13; ++i) poly2[i] = tmp[i];
    }

    /* (1 - z^3 - z^6)^2 */
    poly3[0] = 1;
    for (int t = 0; t < 2; ++t) {
        for (int i = 0; i < 13; ++i) tmp[i] = 0;
        for (int i = 0; i < 13; ++i)
            for (int o = 0; o <= 6; o += 3) {
                int deg = i + o;
                if (deg >= 13) continue;
                seq_t coeff = 1;
                if (o > 0) coeff = -1;
                tmp[deg] += poly3[i] * coeff;
            }
        for (int i = 0; i < 13; ++i) poly3[i] = tmp[i];
    }

    /* (1 - z^6 - z^12) */
    poly4[0] = 1;
    if (6 < 13) poly4[6] = -1;
    if (12 < 13) poly4[12] = -1;

    /* temporary buffers for successive convolutions */
    seq_t temp1[49] = {0}, temp2[49] = {0};

    poly_mul(poly1, 13, poly2, 13, temp1, 49);
    poly_mul(temp1, 49, poly3, 13, temp2, 49);
    poly_mul(temp2, 49, poly4, 13, D, 49);   /* result D[..] */

    /* Allocate and initialise the cache with the first 48 values */
    cache_len = 48;
    seq_cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
    for (size_t i = 0; i < cache_len; ++i)
        seq_cache[i] = init_a[i];
}

/*--------------------------------------------------------------------
   Public function
--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence starts at index 1 */
    init_sequence();

    size_t target = (size_t)n;
    if (target > cache_len) {
        /* extend cache to target terms */
        seq_cache = (seq_t *)realloc(seq_cache, target * sizeof(seq_t));
        for (size_t idx = cache_len; idx < target; ++idx) {
            seq_t sum = 0;
            for (int k = 1; k <= 48; ++k)
                sum += D[k] * seq_cache[idx - k];
            seq_cache[idx] = -sum;   /* recurrence: a_n = -Σ D_k a_{n-k} */
        }
        cache_len = target;
    }
    return seq_cache[n - 1];  /* sequence is 1‑based */
}
