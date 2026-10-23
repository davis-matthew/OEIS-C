/*
 * A162855  Write a prime p in binary. Consider the runs (that alternate
 *          between those of 1's and those of 0's) in this binary
 *          representation. Consider all binary numbers made by rearranging
 *          the runs, leaving each individual run intact, and starting each
 *          such binary number with a run of 1's, followed by a ru...
 * https://oeis.org/A162855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A162855: primes whose every permutation of binary run blocks
   (starting with a run of 1's) yields another prime. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

#define MAX_RUNS 128  /* maximum number of run blocks in any considered prime */

/* Helper: next_permutation for an array of uint8_t, returns true if a new
   permutation was generated, false if the array was the highest. */
static bool next_permutation(uint8_t *a, size_t n)
{
    if (n < 2) return false;
    long long i = (long long)n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) --i;
    if (i < 0) return false;
    long long j = (long long)n - 1;
    while (a[j] <= a[i]) --j;
    uint8_t tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
    long long l = i + 1, r = (long long)n - 1;
    while (l < r) {
        tmp = a[l];
        a[l] = a[r];
        a[r] = tmp;
        ++l; --r;
    }
    return true;
}

/* Comparator for uint8_t used by qsort */
static int cmp_uint8(const void *p1, const void *p2)
{
    uint8_t a = *(const uint8_t *)p1;
    uint8_t b = *(const uint8_t *)p2;
    return (int)a - (int)b;
}

/* Checks whether prime p has the A162855 property. */
static bool valid_prime(seq_t p)
{
    /* Determine bit length of p. */
    size_t bitlen = 0;
    seq_t tmp = p;
    while (tmp != 0) {
        ++bitlen;
        tmp >>= 1;
    }
    if (bitlen == 0) return false;  /* should not happen */

    /* Separate runs of 1s and 0s. */
    uint8_t run1[MAX_RUNS] = {0};
    uint8_t run0[MAX_RUNS] = {0};
    size_t cnt1 = 0, cnt0 = 0;

    bool cur_one = true;
    size_t runlen = 0;
    for (long long i = (long long)bitlen - 1; i >= 0; --i) {
        bool bit = ((p >> (size_t)i) & 1) != 0;
        if (bit == cur_one) {
            ++runlen;
        } else {
            if (cur_one) {
                run1[cnt1++] = (uint8_t)runlen;
            } else {
                run0[cnt0++] = (uint8_t)runlen;
            }
            cur_one = !cur_one;
            runlen = 1;
        }
    }
    /* Final run. */
    if (cur_one) run1[cnt1++] = (uint8_t)runlen;
    else run0[cnt0++] = (uint8_t)runlen;

    /* Work copies for permutations. */
    uint8_t perm1[MAX_RUNS];
    uint8_t perm0[MAX_RUNS];
    uint8_t perm0_orig[MAX_RUNS];
    memcpy(perm1, run1, cnt1 * sizeof(uint8_t));
    memcpy(perm0_orig, run0, cnt0 * sizeof(uint8_t));
    /* Sort the permutations to enable next_permutation. */
    if (cnt1 > 1) qsort(perm1, cnt1, sizeof(uint8_t), cmp_uint8);
    if (cnt0 > 1) qsort(perm0_orig, cnt0, sizeof(uint8_t), cmp_uint8);
    /* Enumerate all permutations of the 1‑run blocks. */
    do {
        /* Restore 0‑run array to original sorted order for each outer iteration. */
        memcpy(perm0, perm0_orig, cnt0 * sizeof(uint8_t));
        /* Enumerate all permutations of the 0‑run blocks. */
        do {
            seq_t val = 0;
            size_t idx1 = 0, idx0 = 0;
            bool one = true;
            /* Alternate between 1 and 0 runs. */
            while (idx1 < cnt1 || idx0 < cnt0) {
                if (one) {
                    size_t len = perm1[idx1++];
                    val = (val << len) | (((seq_t)1 << len) - 1);
                } else {
                    size_t len = perm0[idx0++];
                    val = (val << len);
                }
                one = !one;
            }
            if (!is_prime_big(val)) return false;
        } while (cnt0 > 1 && next_permutation(perm0, cnt0));
    } while (cnt1 > 1 && next_permutation(perm1, cnt1));

    return true;
}

/* Cached sequence values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_candidate = 2;  /* next number to test */

/* Ensure cache has capacity at least 'need'. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();  /* out of memory */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Append a value to the cache. */
static void cache_add(seq_t val)
{
    ensure_cache_capacity(cache_len + 1);
    cache[cache_len++] = val;
}

/* Returns the nth term of A162855 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate;
        next_candidate = cand + 1;

        /* Skip even numbers after 2. */
        if (cand != 2 && ((unsigned long long)cand & 1ULL) == 0ULL) continue;

        if (!is_prime_big(cand)) continue;  /* candidate must be prime */
        if (valid_prime(cand)) cache_add(cand);
    }

    return cache[n - 1];
}
