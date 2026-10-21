/*
 * A066004  Sum of digits of 8^n.
 * https://oeis.org/A066004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = digit_sum(8^n)
 * Sum of the decimal digits of 8 raised to the n‑th power, with offset 0.
 * The implementation keeps a dynamically growing big integer in base 1e9
 * and caches the digit sums up to the largest n requested so far.
 * Complexity: O(n * D(n)) where D(n) ≈ 0.9031 n / 9 ≈ 0.1 n limbs, well within limits.
 */

#include <stdlib.h>
#include <stdint.h>

/* Definition of arbitrary length big integer in base 1e9 */
#define BASE 1000000000ull

/* Static state for caching results */
static seq_t *cache = NULL;          /* computed digit sums a(0)...a(max_n) */
static size_t cache_len = 0;         /* number of cached entries */

static uint32_t *big_digits = NULL;  /* 8^current_n in base BASE, least significant limb first */
static size_t big_len = 0;           /* number of limbs currently used */
static size_t big_cap = 0;           /* current capacity of big_digits array */

/* Ensure capacity for big integer limbs */
static void big_ensure_size(size_t needed)
{
    if (needed <= big_cap) return;
    size_t new_cap = big_cap ? big_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    uint32_t *new_digits = (uint32_t*)realloc(big_digits, new_cap * sizeof(uint32_t));
    if (!new_digits) { abort(); }
    big_digits = new_digits;
    big_cap = new_cap;
}

/* Multiply current big integer by 8 */
static void big_mul8(void)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < big_len; ++i)
    {
        uint64_t prod = (uint64_t)big_digits[i] * 8 + carry;
        big_digits[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    if (carry > 0)
    {
        big_ensure_size(big_len + 1);
        big_digits[big_len++] = (uint32_t)carry;
    }
}

/* Compute sum of decimal digits of current big integer */
static uint64_t big_digit_sum(void)
{
    uint64_t sum = 0;
    for (size_t i = 0; i < big_len; ++i)
    {
        uint32_t v = big_digits[i];
        /* Sum digits of v (0 <= v < BASE=1e9). Each limb has at most 9 digits. */
        while (v > 0)
        {
            sum += v % 10;
            v /= 10;
        }
    }
    return sum;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Lazy initialization on first call */
    if (cache_len == 0)
    {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache_len = 1;
        cache[0] = 1;                /* 8^0 = 1 -> digit sum 1 */

        big_digits = (uint32_t*)malloc(16 * sizeof(uint32_t));
        if (!big_digits) abort();
        big_cap = 16;
        big_len = 1;
        big_digits[0] = 1;            /* big integer = 1 */
    }

    /* Extend cache if needed */
    if ((size_t)n >= cache_len)
    {
        /* Allocate larger cache to hold up to n */
        size_t new_len = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;

        /* Compute missing terms up to n */
        for (size_t k = cache_len; k <= n; ++k)
        {
            big_mul8();                /* big integer *= 8  */
            uint64_t s = big_digit_sum();
            cache[k] = (seq_t)s;       /* store digit sum */
        }
        cache_len = new_len;
    }

    return cache[n];
}
