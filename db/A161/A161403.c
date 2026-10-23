/*
 * A161403  Numbers n such that the count of primes among the permutations of
 *          the digits of n is greater than 3.
 * https://oeis.org/A161403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth integer whose digit permutations contain * more
 *            than 3 primes (ignoring leading zeros), sorted in increasing
 *            order. * The function computes the value for any n ≥ 1, with
 *            memoisation to * support arbitrary call order
 * Range:     u8 none, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   get_digits, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/get_digits.h"
#include "../utils/is_prime_big.h"

/* Generate the next lexicographical permutation of an array of uint8_t.
 * Returns true if a new permutation was produced, false if the array
 * was already in its last permutation. */
static bool next_permutation(uint8_t *arr, size_t n)
{
    if (n <= 1) return false;
    int i = (int)n - 2;
    while (i >= 0 && arr[i] >= arr[i + 1]) i--;
    if (i < 0) return false;
    int j = (int)n - 1;
    while (arr[j] <= arr[i]) j--;
    uint8_t tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
    int l = i + 1, r = (int)n - 1;
    while (l < r) {
        tmp = arr[l];
        arr[l] = arr[r];
        arr[r] = tmp;
        l++; r--;
    }
    return true;
}

/* Count how many distinct primes appear among all permutations of the
 * decimal digits of n. Leading zeros are ignored by construction of the
 * integer from digits. Returns true if this count is greater than 3. */
static bool has_many_primes(seq_t n)
{
    uint8_t digits[128];
    size_t len;
    get_digits(n, digits, &len);

    /* Sort digits to start from the first permutation */
    for (size_t i = 0; i + 1 < len; ++i)
        for (size_t j = i + 1; j < len; ++j)
            if (digits[j] < digits[i]) {
                uint8_t t = digits[i];
                digits[i] = digits[j];
                digits[j] = t;
            }

    int prime_count = 0;
    do {
        seq_t val = 0;
        for (size_t k = 0; k < len; ++k)
            val = val * 10 + digits[k];
        if (is_prime_big(val)) {
            ++prime_count;
            if (prime_count > 3)
                return true;
        }
    } while (next_permutation(digits, len));
    return false;
}

/* Main sequence generator.  The function keeps a cached list of
 * already found terms and continues scanning from the last number
 * examined.  The cache is expanded with realloc, which is wrapped
 * in a cast to satisfy both C and C++ compiler rules. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t used = 0;
    static seq_t last_checked = 0;

    while (used < n) {
        ++last_checked;
        if (has_many_primes(last_checked)) {
            if (used == cap) {
                size_t new_cap = cap ? cap * 2 : 8;
                void *p = realloc(cache, new_cap * sizeof(seq_t));
                if (!p) abort();
#if defined(__cplusplus)
                cache = static_cast<seq_t *>(p);
#else
                cache = (seq_t *)p;
#endif
                cap = new_cap;
            }
            cache[used++] = last_checked;
        }
    }
    return cache[n - 1];
}
