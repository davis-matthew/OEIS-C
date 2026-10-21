/*
 * A074120  Group successively larger composite numbers so that the sum of the
 *          n-th group is a multiple of n. Sequence gives the sum of the terms
 *          in the n-th group.
 * https://oeis.org/A074120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the terms in the n-th group of composite numbers,
 *            where each group sum is a multiple of its index n
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Build an odd‑only sieve up to SIEVE_LIMIT.  */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#define SIEVE_LIMIT 300000000U   /* 3×10^8, enough for n ≤ 10 000 */

static unsigned char *sieve_bits = NULL;   /* bitset of odd numbers > 1 */
static size_t sieve_size = 0;   /* number of bytes allocated */

static void init_sieve(void)
{
    if (sieve_bits != NULL) return;   /* already done */

    /* number of odd numbers from 3 to SIEVE_LIMIT inclusive */
    size_t odd_count = (SIEVE_LIMIT - 1) / 2;
    sieve_size = (odd_count + 7) >> 3;     /* bytes needed */
    sieve_bits = (unsigned char *)malloc(sieve_size);
    if (sieve_bits == NULL) { abort(); }
    for (size_t i = 0; i < sieve_size; ++i) sieve_bits[i] = 0;

    /* helper macros for bitset */
    #define SIEVE_TEST(x) \
        (((x) <= 3) ? 0 : \
        (((x) & 1) ? (sieve_bits[(((x)-3)>>1)>>3] >> ((((x)-3)>>1) & 7) & 1) : 1))
    #define SIEVE_SET(x) \
        do { \
            if (((x) & 1) && ((x) > 3)) { \
                size_t idx = ((x) - 3) >> 1; \
                sieve_bits[idx >> 3] |= 1 << (idx & 7); \
            } \
        } while (0)

    /* sieve odd numbers */
    for (uint32_t i = 3; (uint64_t)i * i <= SIEVE_LIMIT; i += 2) {
        if (!SIEVE_TEST(i)) {
            uint64_t ii = (uint64_t)i * i;
            for (uint64_t j = ii; j <= SIEVE_LIMIT; j += (uint64_t)i << 1) {
                SIEVE_SET((uint32_t)j);
            }
        }
    }

    /* Clean up macro definitions */
    #undef SIEVE_TEST
    #undef SIEVE_SET
}

/* Test whether x is composite (returns 1 if composite, 0 otherwise) */
static inline int is_composite(uint32_t x)
{
    if (x <= 3) return 0;
    if ((x & 1) == 0) return 1;          /* even numbers > 2 are composite */
    size_t idx = (x - 3) >> 1;
    return (sieve_bits[idx >> 3] >> (idx & 7)) & 1;
}

/* Memoization storage for the sequence */
static seq_t *seq_cache = NULL;      /* dynamic array of computed terms */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* allocated capacity of seq_cache */
static uint32_t next_composite = 4;  /* next number to test for compositeness */

/* Ensure seq_cache has at least n elements: compute missing terms. */
static void ensure_terms(idx_t n)
{
    size_t target = (size_t)n;
    while (cache_len < target) {
        idx_t k = (idx_t)(cache_len + 1);  /* group index, 1‑based */
        seq_t sum = 0;
        while (1) {
            /* Find next composite number */
            while (!is_composite(next_composite)) {
                ++next_composite;
            }
            sum += (seq_t)next_composite;
            if ((sum % (seq_t)k) == 0) {
                /* store the sum */
                if (cache_len == cache_cap) {
                    /* grow cache */
                    size_t new_cap = (cache_cap == 0) ? 256 : cache_cap << 1;
                    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
                    if (tmp == NULL) { abort(); }
                    seq_cache = tmp;
                    cache_cap = new_cap;
                }
                seq_cache[cache_len++] = sum;
                ++next_composite;    /* start next group after used number */
                break;
            }
            ++next_composite;
        }
    }
}

/* Main function required by the specification */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        init_sieve();
        initialized = 1;
    }
    ensure_terms(n);
    return seq_cache[(size_t)n - 1];
}
