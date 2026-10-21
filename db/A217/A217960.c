/*
 * A217960  Number of nX4 arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random 0..2 nX4 array.
 * https://oeis.org/A217960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A217960: Number of n×4 0..2 arrays of the minimum of an element
   and its horizontal or antidiagonal neighbours. The sequence satisfies
   the 16‑term linear recurrence
   a(n) = 19*a(n‑1) –23*a(n‑2) –144*a(n‑3) –675*a(n‑4)
          +378*a(n‑5) +3291*a(n‑6) +4937*a(n‑7) –1247*a(n‑8)
          −12054*a(n‑9) −12369*a(n‑10) −5436*a(n‑11) −816*a(n‑12)
          +744*a(n‑13) +364*a(n‑14) +100*a(n‑15)   for n>16
   with the first 16 terms given below. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_from_str.h"

typedef struct {
    bool neg;   /* sign of the magnitude */
    seq_t mag;  /* absolute value */
} signed_big;

static void signed_add_assign(signed_big *s, seq_t val, bool valneg)
{
    if (val == (seq_t)0) return;
    if (s->neg == valneg) {
        s->mag += val;
    } else {
        if (s->mag >= val) {
            s->mag -= val;
        } else {
            s->mag = val - s->mag;
            s->neg = valneg;
        }
    }
}

/* Cache of computed values, 1‑based indexing */
static seq_t *cache = NULL;      /* points to array of seq_t */
static idx_t cache_len = 0;      /* size of allocated array */
static idx_t cache_computed = 0; /* highest n for which cache[n] is valid */

/* Ensure cache has space for indices up to (n-1) inclusive. */
static void ensure_cache_upto(idx_t n)
{
    if (n < cache_len) return;          /* already sufficient */
    idx_t new_len = n + 1;               /* allocate array of size new_len */
    seq_t *new_cache = (seq_t *)malloc((size_t)new_len * sizeof(seq_t));
    if (!new_cache) abort();
    if (cache) {
        for (idx_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_len = new_len;
}

/* Initialise the first sixteen terms from the OEIS data.  */
static void init_cache(void)
{
    if (cache) return;          /* already initialised */
    cache_len = 17;             /* indices 0..16, index 0 unused */
    cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
    if (!cache) abort();

    /* strings for the first sixteen terms */
    static const char *first16_str[16] = {
        "17", "321", "5415", "93765", "1602741",
        "27317977", "465184959", "7919256577",
        "134805742899", "2294680520773", "39060078023831",
        "664879775228995", "11317562137329901",
        "192647146079979077", "3279232808653682467",
        "55818982418231563527"
    };

    for (idx_t i = 1; i <= 16; ++i) {
        cache[i] = seq_from_str(first16_str[i-1]);
    }
    cache_computed = 16;
}

/* Recurrence coefficients, 1‑based indexing; coeffs[0] unused. */
static const int coeffs[17] = {
    0,      /* unused */
    19,   -23,  -144,  -675,   378,   3291,
    4937,  -1247, -12054, -12369, -5436, -816,
     744,   364,   100
};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    init_cache();                        /* make sure cache exists */
    ensure_cache_upto(n);                /* enlarge cache if needed */

    if (n <= cache_computed) return cache[n];

    /* Compute terms from cache_computed+1 up to n using the recurrence. */
    for (idx_t i = cache_computed + 1; i <= n; ++i) {
        signed_big res = { .neg = false, .mag = (seq_t)0 };
        for (int k = 1; k <= 16; ++k) {
            idx_t idx = i - k;
            seq_t val = cache[idx];
            int coeff = coeffs[k];
            int abs_coeff = coeff >= 0 ? coeff : -coeff;
            seq_t prod = val * (seq_t)abs_coeff;
            signed_add_assign(&res, prod, coeff < 0);
        }
        /* The recurrence guarantees a non‑negative result. */
        cache[i] = res.neg ? (seq_t)0 : res.mag;
    }
    cache_computed = n;
    return cache[n];
}
