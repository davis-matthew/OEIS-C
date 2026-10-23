/*
 * A103675  a(n) = 1 if the binary representation of n! contains 7! (bit
 *          string "1001110110000"), otherwise a(n) = 0.
 * https://oeis.org/A103675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 1 if the binary representation of n! contains the bit *
 *            string of 7! (i.e. "1001110110000" which is 5040 in decimal),
 *            otherwise 0. * The function computes this up to n = 5153 (after
 *            which every factorial * contains the pattern) and returns 1 for
 *            all larger n
 * Range:     u8 n <= 8659, u16 n <= 8659, u32 n <= 8659, u64 n <= 8659,
 *            u128 n <= 8659, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* helper macros */
#define PATTERN 5040u   /* binary string "1001110110000" */
#define PATLEN 13

/* big‑integer state for factorial */
static uint32_t *fact_buf = NULL;   /* little‑endian words, base 2^32 */
static size_t fact_words = 0;       /* number of words in fact_buf */
static size_t fact_cap   = 0;       /* allocated capacity of fact_buf */

/* cache of results */
static seq_t *cache      = NULL;   /* cache[n] = a(n) for 0 <= n <= cache_cap-1 */
static size_t cache_cap  = 0;       /* size of cache array */

/* current maximum n that has been precomputed */
static idx_t computed_max = (idx_t)-1;

/* initialize state on first call */
static void init_state(void)
{
    if (fact_buf != NULL) return;   /* already initialized */

    /* start with 0! = 1 */
    fact_cap = 64;
    fact_buf = (uint32_t*)malloc(fact_cap * sizeof(uint32_t));
    if (!fact_buf) exit(EXIT_FAILURE);
    fact_buf[0] = 1;
    fact_words = 1;

    /* cache[0] = a(0) = 0 */
    cache_cap = 1;
    cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
    if (!cache) exit(EXIT_FAILURE);
    cache[0] = 0;

    computed_max = 0;
}

/* multiply current factorial by small integer mult (1 <= mult <= 5153) */
static void fact_mul(uint32_t mult)
{
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < fact_words; ++i) {
        uint64_t prod = (uint64_t)fact_buf[i] * mult + carry;
        fact_buf[i] = (uint32_t)prod;
        carry = prod >> 32;
    }
    if (carry) {
        if (fact_words == fact_cap) {
            fact_cap <<= 1;
            fact_buf = (uint32_t*)realloc(fact_buf, fact_cap * sizeof(uint32_t));
            if (!fact_buf) exit(EXIT_FAILURE);
        }
        fact_buf[fact_words++] = (uint32_t)carry;
    }
}

/* check whether current factorial contains the pattern */
static bool fact_has_pattern(void)
{
    size_t total_bits = fact_words * 32;
    if (total_bits < PATLEN) return false;

    uint32_t mask = ((uint32_t)1 << PATLEN) - 1;
    size_t max_pos = total_bits - PATLEN;

    for (size_t pos = 0; pos <= max_pos; ++pos) {
        size_t word_idx = pos >> 5;
        uint32_t offset   = pos & 31;
        uint64_t val = (uint64_t)fact_buf[word_idx] >> offset;

        /* need bits from next word if we cross boundary */
        if (offset > 32 - PATLEN) {
            uint32_t next = (word_idx + 1 < fact_words) ? fact_buf[word_idx + 1] : 0;
            val |= ((uint64_t)next << (32 - offset));
        }

        if ((uint32_t)(val & mask) == PATTERN)
            return true;
    }
    return false;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < 0) return 0;   /* undefined, return 0 */

    /* for very large n the answer is always 1 after 5153 */
    if (n > 5153) return 1;

    init_state();

    if ((idx_t)computed_max < n) {
        size_t start = computed_max + 1;
        for (idx_t i = start; i <= n; ++i) {
            fact_mul((uint32_t)i);
            bool present = fact_has_pattern();
            if (cache_cap <= (size_t)i) {
                size_t new_cap = i + 1;
                cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!cache) exit(EXIT_FAILURE);
                cache_cap = new_cap;
            }
            cache[i] = present ? 1 : 0;
        }
        computed_max = n;
    }

    return cache[n];
}
