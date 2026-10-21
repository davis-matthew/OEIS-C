/*
 * A084325  Palindromes such that at least one divisor is non-palindromic;
 *          palindromes not in A062687.
 * https://oeis.org/A084325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = nth palindrome that has at least one non‑palindromic
 *            divisor * (i.e. all its proper divisors are not all
 *            palindromes). * The function generates decimal palindromes in
 *            increasing order and * keeps those that satisfy the divisor
 *            condition. * It memoises already found terms to allow
 *            arbitrary...
 * Range:     u8 n <= 8, u16 n <= 598, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* powers of ten up to 10^19 – enough for 64‑bit palindromes and the
 * prefix handling used below. */
static const uint64_t POW10[20] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL,
    1000000ULL, 10000000ULL, 100000000ULL, 1000000000ULL,
    10000000000ULL, 100000000000ULL, 1000000000000ULL,
    10000000000000ULL, 100000000000000ULL, 1000000000000000ULL,
    10000000000000000ULL, 100000000000000000ULL, 1000000000000000000ULL
};

/* reverse the decimal digits of an unsigned 64‑bit integer */
static uint64_t rev_uint64(uint64_t x)
{
    uint64_t r = 0;
    while (x)
    {
        r = r * 10 + x % 10;
        x /= 10;
    }
    return r;
}

/* true iff x is a decimal palindrome (uses 64‑bit arithmetic) */
static bool is_pal_uint64(uint64_t x)
{
    return rev_uint64(x) == x;
}

/* construct the palindrome with given prefix and total length.
 * prefix must have enough digits to cover ceil(len/2). */
static uint64_t make_pal(uint64_t prefix, int len)
{
    int half = (len + 1) / 2;           /* digits in prefix                */
    if (len % 2 == 0)                   /* even length – mirror all digits  */
        return prefix * POW10[half] + rev_uint64(prefix);
    else                                /* odd length – skip middle digit  */
        return prefix * POW10[half - 1] + rev_uint64(prefix / 10);
}

/* test whether palindrome p has at least one divisor that is not a palindrome.
 * Returns true if such a divisor exists; false if every divisor (including 1 and p)
 * is a palindrome (prime palindromes as a special case). */
static bool has_nonpal_divisor(uint64_t p)
{
    for (uint64_t d = 2; d * d <= p; ++d)
    {
        if (p % d == 0)
        {
            uint64_t d2 = p / d;
            if (!is_pal_uint64(d) || !is_pal_uint64(d2))
                return true;
        }
    }
    return false;
}

/* ----- state for generating palindromes incrementally ----- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* current palindrome length being generated */
static int cur_len = 3;          /* start with 3‑digit palindromes        */
/* current prefix (integer representing the first ceil(len/2) digits) */
static uint64_t cur_prefix;
/* last possible prefix for current length */
static uint64_t prefix_end;

/* initialise generator for the first length */
static void init_generator(void)
{
    cur_len = 3;
    int half = (cur_len + 1) / 2;
    cur_prefix = POW10[half - 1];      /* first non‑zero prefix             */
    prefix_end = POW10[half] - 1;
}

/* advance to the next palindrome length */
static void next_length(void)
{
    ++cur_len;
    int half = (cur_len + 1) / 2;
    cur_prefix = POW10[half - 1];
    prefix_end = POW10[half] - 1;
}

/* grow the cache if needed */
static void ensure_cache(void)
{
    if (cache_len >= cache_cap)
    {
        size_t new_cap = (cache_cap == 0) ? 32 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* ----- main sequence function ---------------------------------- */
seq_t a(idx_t n)
{
    if (cache_len == 0)
        init_generator();

    /* generate palindromes until we have at least n terms cached */
    while (cache_len < (size_t)n)
    {
        uint64_t pal = make_pal(cur_prefix, cur_len);
        if (has_nonpal_divisor(pal))
        {
            ensure_cache();
            cache[cache_len++] = (seq_t)pal;
        }

        /* advance to the next palindrome of the current length */
        ++cur_prefix;
        if (cur_prefix > prefix_end)
            next_length();
    }

    /* zero‑based indexing for the cache */
    return cache[(size_t)n - 1];
}
