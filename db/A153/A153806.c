/*
 * A153806  Strobogrammatic cyclops numbers.
 * https://oeis.org/A153806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n-th positive integer that is strobogrammatic (reads
 *            the same upside down using digits 0,1,6,8,9) and contains
 *            exactly one zero digit. The function generates values by
 *            recursively enumerating all such numbers in increasing order
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 341, u64 n <= 8473,
 *            u128 n <= 8473, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* needed headers for memory allocation and size_t */
#include <stdlib.h>
#include <stddef.h>

/* mapping pairs for strobogrammatic digits */
static const int pair_digits[5][2] = {
    {0,0},
    {1,1},
    {6,9},
    {8,8},
    {9,6}
};

/* cache for previously computed sequence terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_gen_length = 0;

/* add a value to the cache, growing it if necessary */
static void add_to_cache(seq_t val) {
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* recursive generator of strobogrammatic numbers of length L
   that contain exactly one zero. 'digits' holds the current digit
   array. */
static void gen_rec(idx_t L, int left, int right, int zeros, int *digits) {
    if (left > right) {                     /* finished building number */
        if (zeros == 1) {
            seq_t val = 0;
            for (int i = 0; i < (int)L; ++i) {
                val = val * (seq_t)10 + (seq_t)digits[i];
            }
            add_to_cache(val);
        }
        return;
    }

    if (left == right) {                    /* middle digit of odd length */
        const int mids[3] = {0, 1, 8};
        for (int k = 0; k < 3; ++k) {
            int d = mids[k];
            digits[left] = d;
            gen_rec(L, left + 1, right - 1, zeros + (d == 0), digits);
        }
    } else {                                /* outer pair of digits */
        for (int idx = 0; idx < 5; ++idx) {
            if (left == 0 && idx == 0) continue;      /* no leading zero */
            int d_left  = pair_digits[idx][0];
            int d_right = pair_digits[idx][1];
            int new_zeros = zeros + (d_left == 0) + (d_right == 0);
            if (new_zeros > 1) continue;               /* prune */
            digits[left] = d_left;
            digits[right] = d_right;
            gen_rec(L, left + 1, right - 1, new_zeros, digits);
        }
    }
}

/* generate sequence terms until we have at least 'needed' elements */
static void generate_more(idx_t needed) {
    for (idx_t L = last_gen_length + 1; ; ++L) {
        int digits[128];                     /* works for all needed lengths */
        gen_rec(L, 0, (int)L - 1, 0, digits);
        last_gen_length = L;
        if (cache_len >= (size_t)needed) break;
    }
}

/* external linkage: returns the n-th term (a(1) = 0) */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1; 0 unused */
    if (cache_len < (size_t)n) generate_more(n);
    return cache[n - 1];
}
