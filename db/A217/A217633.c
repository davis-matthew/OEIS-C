/*
 * A217633  Number of n X 4 arrays of the minimum value of corresponding
 *          elements and their horizontal and vertical neighbors in a random
 *          0..1 n X 4 array.
 * https://oeis.org/A217633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            4*a(n-1)+20*a(n-2)-29*a(n-3)-124*a(n-4)+55*a(n-5)+330*a(n-6) *
 *            -57*a(n-7)-498*a(n-8)-291*a(n-9)+965*a(n-10)+676*a(n-11) *
 *            -991*a(n-12)-659*a(n-13)+771*a(n-14)+610*a(n-15)-581*a(n-16) *
 *            -78*a(n-17)-36*a(n-18)+32*a(n-19)-94*a(n-20)+32*a(n-21) *
 *            +68*a(n-22)-16*a(n-23) * Initial terms (n = 1 …...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache     = NULL;   /* 1‑based index; cache[0] unused   */
static size_t cache_cap = 0;      /* allocated size plus one for index 0 */
static idx_t  cache_len = 0;      /* highest index stored */

/* Initial 23 terms */
static const seq_t init_vals[23] = {
    6ULL,
    38ULL,
    244ULL,
    1418ULL,
    8706ULL,
    52120ULL,
    315378ULL,
    1900838ULL,
    11472148ULL,
    69210290ULL,
    417586442ULL,
    2519466108ULL,
    15201104542ULL,
    91714930026ULL,
    553357475040ULL,
    3338651662490ULL,
    20143579337906ULL,
    121535200857060ULL,
    733276159043790ULL,
    4424182455856594ULL,
    26693068489330768ULL,
    161051201588069830ULL,
    971693816818476190ULL
};

/* Recurrence coefficients c_k for k = 1..23 */
static const int coeffs[23] = {
     4,  20, -29, -124,  55, 330, -57, -498, -291, 965,
     676, -991, -659,  771, 610, -581, -78, -36, 32, -94,
     32, 68, -16
};

/* Ensure cache has at least `need` entries (1‑based).  Allocate or grow
 * the array if necessary, and compute any new values using the
 * recurrence. */
static void ensure_cache(idx_t need)
{
    if (need <= cache_len) return;          /* nothing to do */

    /* Allocate or grow storage to size need+1 (index 0 unused) */
    if ((size_t)need + 1 > cache_cap) {
        size_t new_cap = (size_t)need + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);      /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize missing indices up to at least 23 with the known
     * initial values when required. */
    idx_t start = cache_len + 1;
    if (start <= 23) {
        idx_t end = need < 23 ? need : 23;
        for (idx_t i = start; i <= end; ++i) {
            cache[i] = init_vals[i - 1];
        }
    }

    /* Compute any terms beyond the 23rd using the recurrence. */
    idx_t first_new = start;
    if (first_new < 24) first_new = 24;     /* we already filled up to <=23 */
    for (idx_t n = first_new; n <= need; ++n) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int k = 1; k <= 23; ++k) {
            seq_t term = cache[n - k];
            int c = coeffs[k - 1];
            if (c > 0)
                pos += term * (seq_t)c;
            else if (c < 0)
                neg += term * (seq_t)(-c);
        }
        cache[n] = pos - neg;   /* pos is always >= neg for this sequence */
    }

    cache_len = need;
}

/* Public function computing the sequence term at index n. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* sequence is defined for n >= 1 */
    ensure_cache(n);
    return cache[n];
}
