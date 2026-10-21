/*
 * A073632  Numbers k such that floor((3/2)^k) = A002379(k) is odd.
 * https://oeis.org/A073632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth integer k such that floor((3/2)^k) is odd
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Big‑integer representation of 3^k as an array of 64‑bit limbs (little‑endian). */
static uint64_t *bits = NULL;
static size_t bits_len = 0;   /* number of limbs currently used */
static size_t bits_cap = 0;   /* allocated capacity in limbs */
static idx_t processed_k = 0;
static int big_initialized = 0;

static void ensure_cache_cap(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

static void ensure_bits_cap(size_t needed)
{
    if (bits_cap < needed) {
        size_t new_cap = bits_cap ? bits_cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        bits = (uint64_t *)realloc(bits, new_cap * sizeof(uint64_t));
        bits_cap = new_cap;
    }
}

static void multiply_by_3(void)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < bits_len; ++i) {
        unsigned __int128 prod = (unsigned __int128)bits[i] * 3 + carry;
        bits[i] = (uint64_t)prod;
        carry   = (uint64_t)(prod >> 64);
    }
    if (carry) {
        ensure_bits_cap(bits_len + 1);
        bits[bits_len++] = carry;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not used for the offset */

    /* One‑time initialization */
    if (!big_initialized) {
        ensure_bits_cap(1);
        bits_len = 1;
        bits[0] = 1;        /* 3^0 */
        processed_k = 0;
        big_initialized = 1;
    }
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    while (cache_len < (size_t)n) {
        /* Test parity of floor((3/2)^processed_k) == (3^k >> k) & 1 */
        size_t word  = processed_k >> 6;        /* processed_k / 64 */
        size_t shift = processed_k & 63;       /* processed_k % 64 */
        int odd = 0;
        if (word < bits_len) {
            odd = ((bits[word] >> shift) & 1ULL) != 0;
        }

        if (odd) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)processed_k;
        }

        /* Prepare next value: 3^(k+1) */
        multiply_by_3();
        ++processed_k;
    }

    return cache[n - 1];
}
