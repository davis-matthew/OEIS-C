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
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   get_digits, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/get_digits.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
                void *p = mem_resize(seq_t, cache, new_cap);
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
