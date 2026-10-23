/*
 * A134793  Concatenation of next n nonprime numbers.
 * https://oeis.org/A134793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/*   Concatenation of next n nonprime numbers (1 is counted as nonprime).   */

/*  External types supplied by the build system */
extern seq_t a(idx_t n);

/* ---------- constants and static state ---------- */
static seq_t *a_cache = NULL;        /* cache of computed a(n) values   */
static size_t a_cap   = 0;           /* allocated size of a_cache       */
static idx_t   a_max   = 0;          /* highest n that has been computed */

static idx_t   test_candidate = 2;   /* next integer to test for nonprime  */
static bool    first_used    = false;/* whether 1 has already been used     */

static uint8_t *comp_bits  = NULL;   /* bitset: 1 = composite, 0 = prime or 0/1 */
static idx_t   comp_limit   = 0;     /* highest number covered by comp_bits */

static seq_t  pow10[21];             /* pre‑computed powers of 10       */
static bool   pow10_inited = false;

/* ---------- helper functions ---------- */
static inline void init_pow10(void)
{
    if (pow10_inited) return;
    pow10[0] = (seq_t)1;
    for (int i = 1; i < 21; ++i)
        pow10[i] = pow10[i - 1] * (seq_t)10;
    pow10_inited = true;
}

static inline int digit_len_idx(idx_t x)
{
    if (x == 0) return 1;
    int d = 0;
    while (x) { ++d; x /= 10; }
    return d;
}

static inline bool isComposite(idx_t x)
{
    return ((comp_bits[x >> 3] >> (x & 7)) & 1u) != 0;
}

/* Builds a sieve that marks composites up to 'limit'.  The caller must
   free previous comp_bits if realloc is used. */
static void build_sieve(idx_t limit, uint8_t **bits, idx_t *lim)
{
    size_t bytes = (limit >> 3) + 1;
    *bits = (uint8_t *)realloc(*bits, bytes);
    if (!*bits) exit(1);
    memset(*bits, 0, bytes);

    /* Treat 0 and 1 as composite */
    (*bits)[0] |= 1u << 0;   /* 0 */
    (*bits)[0] |= 1u << 1;   /* 1 */

    for (idx_t i = 2; i <= limit / i; ++i) {
        if (!isComposite(i)) {
            for (idx_t j = i * i; j <= limit; j += i)
                (*bits)[j >> 3] |= 1u << (j & 7);
        }
    }
    *lim = limit;
}

/* Ensures the sieve covers at least 'need' */
static void ensure_sieve(idx_t need)
{
    if (comp_limit >= need) return;
    idx_t new_limit = comp_limit > 0 ? comp_limit : 1000;
    while (new_limit < need) {
        new_limit <<= 1;
        if (new_limit < 1000) new_limit = 1000;
    }
    free(comp_bits);
    build_sieve(new_limit, &comp_bits, &comp_limit);
}

/* Returns the next nonprime number in the sequence, updating internal
   state.  The first nonprime number is 1. */
static inline seq_t next_nonprime(void)
{
    if (!first_used) {
        first_used = true;
        return (seq_t)1;
    }
    /* Find the next composite number */
    while (true) {
        ensure_sieve(test_candidate);
        if (isComposite(test_candidate))
            break;
        ++test_candidate;
    }
    seq_t val = (seq_t)test_candidate;
    ++test_candidate;
    return val;
}

/* Ensures the a_cache has capacity for at least 'n' terms */
static void ensure_cache_capacity(idx_t n)
{
    if (a_cap >= (size_t)n) return;
    size_t new_cap = a_cap ? a_cap * 2 : 16;
    while (new_cap < (size_t)n) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);
    a_cache = new_cache;
    a_cap   = new_cap;
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* undefined for offset 0 */

    init_pow10();
    ensure_cache_capacity(n);

    if (n <= a_max) return a_cache[n - 1];

    /* Compute terms from a_max+1 up to n */
    for (idx_t cur = a_max + 1; cur <= n; ++cur) {
        seq_t val = (seq_t)0;
        for (idx_t i = 0; i < cur; ++i) {
            seq_t next = next_nonprime();
            int d = digit_len_idx((idx_t)next);
            val = val * pow10[d] + next;
        }
        a_cache[cur - 1] = val;
    }
    a_max = n;
    return a_cache[n - 1];
}
