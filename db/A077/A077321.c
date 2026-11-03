/*
 * A077321  Rearrange primes so as to form a triangle in which n-th row
 *          contains the n smallest primes == 1 (mod n) which have not
 *          occurred earlier.
 * https://oeis.org/A077321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 2143, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Validated: standard, pass, 75 terms through n = 4974 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A077321: Arrange primes into a triangle where row n contains the
 * n smallest unused primes ≡ 1 (mod n). The flattened rows form the sequence.
 * This implementation builds rows incrementally, memoizes all terms, and
 * tracks used primes with a dynamic bitset.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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

/* ----------- bitset for used primes ----------- */
static uint8_t *used_bits = NULL;   /* bit i set -> prime i is used */
static size_t used_bytes = 0;       /* number of bytes allocated */

/* Ensure used_bits can hold bit for value 'val' */
static void ensure_used_capacity(uint64_t val)
{
    size_t idx = val >> 3;               /* byte index */
    if (idx + 1 <= used_bytes) return;

    size_t new_bytes = used_bytes ? used_bytes << 1 : 256;
    while (new_bytes <= idx + 1) new_bytes <<= 1;

    uint8_t *tmp = (uint8_t *)realloc(used_bits, new_bytes);
    if (!tmp) abort();
    /* zero any newly allocated bytes */
    memset(tmp + used_bytes, 0, new_bytes - used_bytes);
    used_bits = tmp;
    used_bytes = new_bytes;
}

/* Test if value 'val' is marked used */
static inline bool is_used(uint64_t val)
{
    if (val == 0) return false;
    size_t idx = val >> 3;
    if (idx >= used_bytes) return false;
    return (used_bits[idx] >> (val & 7)) & 1;
}

/* Mark value 'val' as used */
static inline void set_used(uint64_t val)
{
    if (val == 0) return;
    ensure_used_capacity(val);
    used_bits[val >> 3] |= (1 << (val & 7));
}

/* ----------- sequence cache ----------- */
static seq_t *seq_cache = NULL;  /* array of computed terms (0‑based) */
static size_t seq_cap   = 0;     /* allocated capacity of seq_cache */
static size_t seq_len   = 0;     /* number of terms currently stored */

/* Allocate more space for seq_cache if needed */
static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
    if (!tmp) abort();
    seq_cache = tmp;
    seq_cap = new_cap;
}

/* Append a new term to the sequence cache */
static void seq_append(uint64_t val)
{
    ensure_seq_capacity(seq_len + 1);
    seq_cache[seq_len++] = (seq_t)val;
}

/* ----------- incremental row generation ----------- */
static idx_t next_row = 1;   /* next row number to process */

/* Process rows until seq_len >= target */
static void generate_up_to(idx_t target)
{
    while ((idx_t)seq_len < target)
    {
        uint64_t m = (uint64_t)next_row;      /* current row number */
        uint64_t needed = m;                  /* how many primes this row needs */
        uint64_t collected = 0;

        if (m == 1)
        {
            /* First row: smallest unused prime (2) */
            for (uint64_t cand = 2; collected < needed; ++cand)
            {
                if (is_prime_big((seq_t)cand) && !is_used(cand))
                {
                    set_used(cand);
                    seq_append(cand);
                    ++collected;
                }
            }
        }
        else
        {
            /* Row m: primes ≡ 1 (mod m) */
            for (uint64_t k = 1; collected < needed; ++k)
            {
                uint64_t cand = m * k + 1;
                if (is_prime_big((seq_t)cand) && !is_used(cand))
                {
                    set_used(cand);
                    seq_append(cand);
                    ++collected;
                }
            }
        }

        ++next_row;
    }
}

/* ----------- required function ----------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n < 1, guard anyway */

    generate_up_to(n);                /* ensure at least n terms are computed */
    return seq_cache[n - 1];          /* return the nth term (1‑based) */
}
