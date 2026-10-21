/*
 * A055063  Positions of primes in A054636 with offset 1.
 * https://oeis.org/A055063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Positions of primes in the cumulative sum of base‑8 digits.
   a(n) is the 1‑based index of the nth prime that occurs in the
   running sum of the octal digits of the non‑negative integers
   (0,1,2,3,…).  */
   
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ----------  deterministic Miller–Rabin  --------------------------------- */
static inline uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    /* use 128‑bit intermediate to avoid overflow */
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

static inline uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1ULL)
            result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2U)                 return false;
    /* trial division by small primes */
    static const uint64_t small[] =
        {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
         19ULL, 23ULL, 29ULL, 31ULL, 37ULL};
    for (size_t i = 0; i < sizeof(small) / sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n % p == 0U) return n == p;
    }

    /* write n-1 as d * 2^s */
    uint64_t d = n - 1U;
    uint64_t s = 0U;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1ULL;
        ++s;
    }

    /* deterministic bases for 64‑bit integers */
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL,
         450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0ULL) return true;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1ULL || x == n - 1ULL) continue;
        bool cont = false;
        for (uint64_t r = 1ULL; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1ULL) {
                cont = true;
                break;
            }
        }
        if (!cont) return false;
    }
    return true;
}

/* ----------  sequence generator ------------------------------------------ */
seq_t a(idx_t n)
{
    /* offset is 1 – a(1) is the first term */
    if (n == 0U) return 0U;

    /* static storage for previously found positions */
    static seq_t *positions = NULL;
    static size_t pos_cap  = 0U;
    static size_t pos_len  = 0U;      /* number of primes found */

    /* generation state */
    static uint64_t current_num = 0U;   /* next natural number to process */
    static uint64_t pos_counter = 0U;   /* 1‑based position counter */
    static uint64_t sum = 0U;           /* running sum of digits */

    /* allocate storage on first call */
    if (positions == NULL) {
        pos_cap = 256U;
        positions = (seq_t *)malloc(pos_cap * sizeof(seq_t));
        pos_len = 0U;
    }

    /* generate until we have at least n primes found */
    while (pos_len < n) {
        /* obtain octal digits of current_num */
        uint8_t digits[32];
        size_t len = 0U;
        uint64_t t = current_num;

        if (t == 0ULL) {
            digits[0] = 0U;
            len = 1U;
        } else {
            while (t > 0ULL) {
                digits[len++] = (uint8_t)(t % 8ULL);
                t /= 8ULL;
            }
            /* reverse to most‑significant first */
            for (size_t i = 0U, j = len - 1U; i < j; ++i, --j) {
                uint8_t tmp = digits[i];
                digits[i] = digits[j];
                digits[j] = tmp;
            }
        }

        /* process each digit */
        for (size_t i = 0U; i < len; ++i) {
            ++pos_counter;
            sum += (uint64_t)digits[i];
            if (is_prime_u64(sum)) {
                if (pos_len == pos_cap) {
                    pos_cap <<= 1U; /* double capacity */
                    positions = (seq_t *)realloc(positions,
                                                 pos_cap * sizeof(seq_t));
                }
                positions[pos_len++] = (seq_t)pos_counter;
            }
        }

        ++current_num;
    }

    return positions[n - 1U];
}
