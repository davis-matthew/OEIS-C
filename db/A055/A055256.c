/*
 * A055256  Number of odd digits in 3^n.
 * https://oeis.org/A055256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of odd decimal digits of 3^n
 * Range:     u8 n <= 991, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- big integer in base 10 (least‑significant digit first) ---------- */
static uint8_t *big_digits = NULL;
static size_t   big_len    = 0;
static size_t   big_cap    = 0;

static void ensure_big_cap(size_t min_cap)
{
    if (big_cap >= min_cap) return;
    size_t new_cap = big_cap ? big_cap * 2 : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    uint8_t *tmp = (uint8_t*)realloc(big_digits, new_cap);
    if (!tmp) exit(1);
    big_digits = tmp;
    big_cap    = new_cap;
}

static void multiply_by_three(void)
{
    uint32_t carry = 0;
    for (size_t i = 0; i < big_len; ++i) {
        uint32_t prod = (uint32_t)big_digits[i] * 3 + carry;
        big_digits[i] = (uint8_t)(prod % 10);
        carry = prod / 10;
    }
    while (carry) {
        ensure_big_cap(big_len + 1);
        big_digits[big_len++] = (uint8_t)(carry % 10);
        carry /= 10;
    }
}

static uint64_t count_odd_digits(void)
{
    uint64_t c = 0;
    for (size_t i = 0; i < big_len; ++i)
        if (big_digits[i] & 1) ++c;
    return c;
}

/* ---------- cache of a(n) values ---------- */
static seq_t *counts     = NULL;
static size_t counts_cap = 0;   /* allocated capacity */
static idx_t  cached_n   = 0;   /* largest n that has been computed */

static void ensure_counts_capacity(size_t min_index)
{
    if (counts == NULL) {
        counts_cap = 16;
        counts = (seq_t*)realloc(NULL, counts_cap * sizeof(seq_t));
        if (!counts) exit(1);
    }
    if (counts_cap <= (size_t)min_index) {
        size_t new_cap = counts_cap ? counts_cap * 2 : 16;
        while (new_cap <= (size_t)min_index) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(counts, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        counts = tmp;
        counts_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (counts == NULL) {
        /* initialise big integer 3^0 = 1 */
        ensure_big_cap(1);
        big_digits[0] = 1;
        big_len = 1;

        /* initialise cache for index 0 */
        ensure_counts_capacity(0);
        counts[0] = (seq_t)1;  /* a(0) = 1 */
        cached_n = 0;
    }

    if (cached_n < n) {
        for (idx_t k = cached_n + 1; k <= n; ++k) {
            multiply_by_three();
            uint64_t odd_cnt = count_odd_digits();
            ensure_counts_capacity(k);
            counts[k] = (seq_t)odd_cnt;
        }
        cached_n = n;
    }
    return counts[n];
}
