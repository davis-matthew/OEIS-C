/*
 * A075083  Largest composite number formed by permuting the digits of n, or 0
 *          if no such number exists.
 * https://oeis.org/A075083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = largest composite number obtained by permuting the digits
 *            of n, or 0 if no such composite exists
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t digits_to_value(const int *dig, int len)
{
    uint64_t val = 0;
    for (int i = 0; i < len; ++i) val = val * 10 + (uint64_t)dig[i];
    return val;
}

/* next permutation in lexicographic order.  Returns true if a next
   permutation exists, false if the array is at its last permutation.   */
static bool next_perm(int *a, int n) {
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) --i;
    if (i < 0) return false;

    int j = n - 1;
    while (a[j] <= a[i]) --j;

    int tmp = a[i]; a[i] = a[j]; a[j] = tmp;

    for (int l = i + 1, r = n - 1; l < r; ++l, --r) {
        tmp = a[l]; a[l] = a[r]; a[r] = tmp;
    }
    return true;
}

/* is_prime checks whether x is a prime number.  Works up to 2^64 */
static bool is_prime(uint64_t x)
{
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if (x % 2 == 0 || x % 3 == 0) return false;
    uint64_t i = 5;
    uint64_t w = 2;
    while (i * i <= x) {
        if (x % i == 0) return false;
        i += w;
        w = 6 - w;  /* alternate 2,4 increments */
    }
    return true;
}

static bool is_composite(uint64_t x)
{
    return x >= 4 && !is_prime(x);
}

/* next_permutation on an array of length n in place.  Returns false when
   no further permutation exists.  It follows C++ std::next_permutation
   semantics for ascending ordered array. */

static int digit_count(uint64_t v)
{
    int cnt = 0;
    do { ++cnt; v /= 10; } while (v);
    return cnt;
}

seq_t a(idx_t n)
{
    /* Convert idx_t (uint64_t) to uint64_t for digit extraction.
       Assume n fits in 64 bits for the tests that matter.  */
    uint64_t up = (uint64_t)n;
    if (up == 0) return (seq_t)0;   /* not expected, but safe */

    /* Extract digits of n into array */
    int dcnt = digit_count(up);
    int digits[20];  /* enough for 64-bit numbers */
    for (int i = dcnt - 1; i >= 0; --i) {
        digits[i] = up % 10;
        up /= 10;
    }

    /* Sort digits ascending to start with smallest permutation */
    for (int i = 0; i < dcnt - 1; ++i) {
        for (int j = i + 1; j < dcnt; ++j) {
            if (digits[i] > digits[j]) {
                int t = digits[i]; digits[i] = digits[j]; digits[j] = t;
            }
        }
    }

    uint64_t best = 0;

    /* Evaluate the first permutation */
    uint64_t val = digits_to_value(digits, dcnt);
    if (is_composite(val) && val > best) best = val;

    /* Generate all other permutations */
    while (next_perm(digits, dcnt)) {
        val = digits_to_value(digits, dcnt);
        if (is_composite(val) && val > best) best = val;
    }

    return (seq_t)best;
}
