/*
 * A210757  Primes less than 100 sorted lexicographically in decimal
 *          representation.
 * https://oeis.org/A210757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A210757: the primes less than 100 sorted lexicographically
 * (decimal representation).  The function builds the sequence
 * once and caches it, then returns the nth term (1‑based). */

/* Note: SEQ_LEN is a constant integral expression, so the
 * array can be declared at file scope. */
#define SEQ_LEN 25

#include <stdbool.h>
#include <stddef.h>

/* Return the lexicographic comparison of two numbers < 100.
 * The comparison is performed on their decimal string representations
 * without constructing actual strings. */
static int lex_cmp(seq_t a, seq_t b)
{
    if (a == b) return 0;

    /* Determine first digit and length. */
    int a_first = (a < 10) ? (int)a : (int)(a / 10);
    int b_first = (b < 10) ? (int)b : (int)(b / 10);
    int a_len   = (a < 10) ? 1 : 2;
    int b_len   = (b < 10) ? 1 : 2;

    if (a_first < b_first) return -1;
    if (a_first > b_first) return  1;

    /* first digits are equal */
    if (a_len < b_len) return -1;   /* a is a prefix */
    if (a_len > b_len) return  1;   /* a is longer */

    /* both are two-digit numbers, compare the second digit */
    int a_second = (int)(a % 10);
    int b_second = (int)(b % 10);
    if (a_second < b_second) return -1;
    if (a_second > b_second) return 1;

    return 0; /* should never reach here for distinct a,b */
}

/* Static cache of sorted primes. */
static seq_t sorted[SEQ_LEN];
static bool initialized = false;

/* Initialise the cache: sieve primes < 100 and insert them
 * into 'sorted' in lexicographic order. */
static void init_cache(void)
{
    if (initialized) return;

    bool is_prime[100];
    for (int i = 2; i < 100; ++i) is_prime[i] = true;
    for (int p = 2; p * p < 100; ++p) {
        if (!is_prime[p]) continue;
        for (int m = p * p; m < 100; m += p)
            is_prime[m] = false;
    }

    int count = 0;
    for (int n = 2; n < 100; ++n) {
        if (!is_prime[n]) continue;
        seq_t val = (seq_t)n;

        /* Find insertion position maintaining lexicographic order. */
        int pos = 0;
        while (pos < count && lex_cmp(sorted[pos], val) < 0) ++pos;

        /* Shift elements to make room. */
        for (int j = count; j > pos; --j) sorted[j] = sorted[j - 1];
        sorted[pos] = val;
        ++count;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    init_cache();

    /* n is 1‑based; return 0 for out of bounds. */
    if (n < 1 || n > SEQ_LEN) return 0;

    return sorted[(size_t)(n - 1)];
}
