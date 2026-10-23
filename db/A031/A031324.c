/*
 * A031324  Decimal digits of successive Fibonacci numbers.
 * https://oeis.org/A031324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A031324: decimal digits of successive Fibonacci numbers.
 *  a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=5, a(6)=8, a(7)=1, a(8)=3, ...
 *
 *  The value a(n) is the nth digit of the infinite concatenation
 *  0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, ...
 *  The algorithm generates Fibonacci numbers as big integers
 *  (decimal‑digit representation) and appends each digit to a
 *  growing cache until the requested index has been reached.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Simple representation of a large integer as a little‑endian array
 * of decimal digits. */
typedef struct {
    uint8_t *digits;   /* least‑significant digit first */
    size_t   len;      /* number of digits */
} BigInt;

/* Construct a BigInt from an unsigned integer (only used for 0 and 1). */
static void big_from_uint(BigInt *b, uint64_t v)
{
    b->len    = 0;
    b->digits = (uint8_t*)malloc(1);
    if (!b->digits) abort();
    do {
        b->digits[b->len++] = (uint8_t)(v % 10);
        v /= 10;
    } while (v > 0);
}

/* Add two BigInts and return the result.  The caller must free the
 * resulting BigInt's digits. */
static BigInt big_add(const BigInt *a, const BigInt *b)
{
    BigInt r;
    size_t i, maxlen = a->len > b->len ? a->len : b->len;
    r.digits = (uint8_t*)malloc(maxlen + 1);
    if (!r.digits) abort();
    uint8_t carry = 0;
    for (i = 0; i < maxlen; ++i) {
        uint8_t da = (i < a->len) ? a->digits[i] : 0;
        uint8_t db = (i < b->len) ? b->digits[i] : 0;
        uint8_t s  = da + db + carry;
        r.digits[i] = s % 10;
        carry = s / 10;
    }
    if (carry) {
        r.digits[i++] = carry;
    }
    r.len = i;
    return r;
}

/* Append the decimal digits of a BigInt to the global digit cache.
 * The digits are appended most‑significant first. */
static void append_big_digits(const BigInt *b, seq_t **cache,
                              size_t *used, size_t *cap)
{
    /* Ensure enough space. */
    if (*used + b->len > *cap) {
        size_t newcap = *cap ? (*cap << 1) : 512;
        seq_t *tmp = (seq_t*)realloc(*cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        *cache = tmp;
        *cap   = newcap;
    }
    /* Most significant digit first. */
    for (size_t i = b->len; i-- > 0; ) {
        (*cache)[(*used)++] = (seq_t)b->digits[i];
    }
}

/* Return the nth digit of the concatenated Fibonacci numbers. */
seq_t a(idx_t n)
{
    static seq_t  *digit_cache = NULL;
    static size_t  cache_len   = 0, cache_cap   = 0;
    static BigInt  fprev, fcurr;
    static bool    initialized = false;

    if (!initialized) {
        /* Seed the first two Fibonacci numbers: 0 and 1. */
        fprev.digits = (uint8_t*)malloc(1);
        if (!fprev.digits) abort();
        fprev.len = 1;
        fprev.digits[0] = 0;

        big_from_uint(&fcurr, 1);

        /* Allocate initial digit cache. */
        digit_cache = (seq_t*)malloc(512 * sizeof(seq_t));
        if (!digit_cache) abort();
        cache_cap   = 512;
        cache_len   = 0;

        /* Store digits of 0 and 1. */
        append_big_digits(&fprev, &digit_cache, &cache_len, &cache_cap);
        append_big_digits(&fcurr, &digit_cache, &cache_len, &cache_cap);

        initialized = true;
    }

    /* Generate further digits until we have at least n+1 of them. */
    while (cache_len <= (size_t)n) {
        /* Next Fibonacci number = fprev + fcurr. */
        BigInt next = big_add(&fprev, &fcurr);

        /* Append its digits. */
        append_big_digits(&next, &digit_cache, &cache_len, &cache_cap);

        /* Advance the fibonacci pair, freeing the old fprev digits. */
        free(fprev.digits);
        fprev = fcurr;
        fcurr = next;
    }

    return digit_cache[n];
}
