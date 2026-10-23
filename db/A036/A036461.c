/*
 * A036461  Number of 1 digits in base 3 representation of 2^n.
 * https://oeis.org/A036461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1088, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of 1–digits in the base‑3 representation of 2^n,
   i.e. the OEIS sequence A036461.  The algorithm successively
   multiplies the base‑3 digits of 2^k by 2 to obtain the digits of
   2^(k+1) and counts the 1’s.  Results are memoised so that each
   n is evaluated only once, regardless of call order. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static storage for memoisation */
    static seq_t *cache = NULL;      /* a(i)  for 0 ≤ i ≤ computed_n  */
    static size_t cache_cap = 0;     /* allocated size of cache */
    static unsigned char *digits = NULL; /* base‑3 digits of 2^k */
    static size_t digits_len = 0;     /* current number of digits */
    static size_t digits_cap = 0;     /* allocated size of digits */
    static size_t computed_n = 0;     /* highest n that has been computed */

    /* Initialise on first call */
    if (cache == NULL) {
        /* Allocate for a(0) */
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;          /* 2^0 = 1, one ‘1’ digit */

        /* Initialise base‑3 representation for 2^0 */
        digits_cap = 1;
        digits = (unsigned char *)malloc(sizeof(unsigned char) * digits_cap);
        digits[0] = 1;                /* single digit ‘1’ */
        digits_len = 1;
        computed_n = 0;
    }

    size_t target = (size_t)n;
    if (target <= computed_n)
        return cache[target];

    /* Extend cache up to target, computing missing values */
    while (computed_n < target) {
        /* Ensure there is space for the new digit that may appear */
        if (digits_len >= digits_cap) {
            size_t new_cap = digits_cap * 2;
            if (new_cap == 0) new_cap = 1;
            unsigned char *tmp = (unsigned char *)realloc(
                digits, sizeof(unsigned char) * new_cap);
            if (!tmp) return 0;   /* allocation failure */
            digits = tmp;
            digits_cap = new_cap;
        }

        /* Multiply the current number by 2 (base‑3 digits) */
        unsigned int carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            unsigned int tmp = digits[i] * 2u + carry;
            digits[i] = (unsigned char)(tmp % 3u);
            carry     = tmp / 3u;
        }
        while (carry) {
            if (digits_len >= digits_cap) {
                size_t new_cap = digits_cap * 2;
                if (new_cap == 0) new_cap = 1;
                unsigned char *tmp = (unsigned char *)realloc(
                    digits, sizeof(unsigned char) * new_cap);
                if (!tmp) return 0;
                digits = tmp;
                digits_cap = new_cap;
            }
            digits[digits_len++] = (unsigned char)(carry % 3u);
            carry /= 3u;
        }

        /* Count the 1‑digits in the new representation */
        seq_t count = 0;
        for (size_t i = 0; i < digits_len; ++i)
            if (digits[i] == 1u)
                count++;

        /* Store the count in the cache */
        size_t next_index = computed_n + 1;
        if (next_index >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap == 0) new_cap = 1;
            seq_t *tmp = (seq_t *)realloc(cache,
                                          sizeof(seq_t) * new_cap);
            if (!tmp) return 0;
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[next_index] = count;
        computed_n = next_index;
    }

    return cache[target];
}
