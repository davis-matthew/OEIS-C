/*
 * A238690  Let each integer m (1 <= m <= n) be factorized as m =
 *          prime_m(1)*prime_m(2)*...*prime_m(bigomega(m)), with the primes
 *          sorted in nonincreasing order. Then a(n) is the number of values
 *          of m such that each prime_m(i) <= prime_n(i).
 * https://oeis.org/A238690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of m (1 ≤ m ≤ n) whose descending prime-factor
 *            list is componentwise ≤ that of n
 * Range:     u8 n <= 1328, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *spfs = NULL;          /* smallest prime factor for each number */
static uint8_t   *lens  = NULL;        /* number of prime factors (bigomega) */
static size_t   *starts = NULL;        /* start index of prime list in flat array */
static int       *flat  = NULL;        /* flattened list of primes, sorted descending */
static seq_t    *cache = NULL;         /* computed sequence values */
static idx_t     cache_size = 0;       /* max n for which cache is filled */

/* ---------- helper to precompute data up to max_n ---------- */
static void build_upto(idx_t max_n)
{
    /* free old data */
    free(spfs);  free(lens);  free(starts);  free(flat);  free(cache);
    spfs = (uint32_t*)calloc((size_t)max_n + 1, sizeof(uint32_t));
    lens = (uint8_t*)calloc((size_t)max_n + 1,  sizeof(uint8_t));
    starts = (size_t*)calloc((size_t)max_n + 1, sizeof(size_t));

    /* sieve for smallest prime factor */
    for (idx_t i = 2; i <= max_n; ++i) {
        if (!spfs[i]) {
            spfs[i] = (uint32_t)i;
            if (i * i <= max_n)
                for (idx_t j = i * i; j <= max_n; j += i)
                    if (!spfs[j]) spfs[j] = (uint32_t)i;
        }
    }
    spfs[1] = 1;

    /* bigomega (number of prime factors, counted with multiplicity) */
    lens[1] = 0;
    for (idx_t i = 2; i <= max_n; ++i)
        lens[i] = lens[i / spfs[i]] + 1;

    /* compute total length needed for flat array */
    size_t total_len = 0;
    for (idx_t i = 1; i <= max_n; ++i) total_len += lens[i];
    flat = (int*)malloc(total_len * sizeof(int));

    /* fill flat array with descending prime lists */
    size_t pos = 0;
    int tmp[20]; /* maximum prime factors for 10000 is < 20 */
    for (idx_t i = 1; i <= max_n; ++i) {
        starts[i] = pos;
        idx_t x = i;
        int cnt = 0;
        while (x > 1) {
            int p = spfs[x];
            tmp[cnt++] = p;
            x /= p;
        }
        for (int j = cnt - 1; j >= 0; --j)
            flat[pos++] = tmp[j];
        lens[i] = cnt; /* already set, but keep for clarity */
    }

    /* compute sequence values */
    cache = (seq_t*)malloc((size_t)max_n * sizeof(seq_t));
    for (idx_t i = 1; i <= max_n; ++i) {
        seq_t count = 0;
        uint8_t len_i = lens[i];
        size_t start_i = starts[i];
        for (idx_t m = 1; m <= i; ++m) {
            uint8_t len_m = lens[m];
            if (len_m > len_i) continue;         /* cannot cover if more factors */
            size_t start_m = starts[m];
            bool ok = true;
            for (uint8_t k = 0; k < len_m; ++k) {
                if (flat[start_m + k] > flat[start_i + k]) { ok = false; break; }
            }
            if (ok) ++count;
        }
        cache[i-1] = count;
    }
    cache_size = max_n;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not defined before offset */
    if (n < cache_size) return cache[n-1];
    build_upto(n);
    return cache[n-1];
}
