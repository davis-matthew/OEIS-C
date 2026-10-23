/*
 * A025285  Numbers that are the sum of 2 nonzero squares in exactly 2 ways.
 * https://oeis.org/A025285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer that can be written * as a^2 +
 *            b^2 (with a, b > 0) in exactly two distinct unordered * ways.
 *            The order and signs are ignored, e.g. 50 = 5^2+5^2 = 1^2+7^2. *
 *            The implementation precomputes all sums up to a growing bound *
 *            until enough terms are found
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Default bound for creating the first table.  10 million comfortably
 * contains more than ten‑thousand terms, and the memory footprint of
 * the frequency array (16 bit counters) is about 20 MB. */
#define DEFAULT_LIMIT 10000000ULL

/* Static state */
static bool          initialized = false;
static seq_t        *term_list   = NULL;
static size_t        term_len    = 0;   /* number of terms stored */
static size_t        term_cap    = 0;   /* allocated capacity of term_list */
static idx_t         current_limit = 0;

/* Free the existing table and set state to uninitialized. */
static void clear_tables(void)
{
    free(term_list);
    term_list = NULL;
    term_len  = 0;
    term_cap  = 0;
    initialized = false;
    current_limit = 0;
}

/* Build the table using the given limit.  All values up to
 * 'limit' are considered.  Returns true on success. */
static bool build_table(idx_t limit)
{
    /* Frequency of unordered representations (a <= b).  16‑bit is
     * enough because the maximum number of representations below
     * 10 million is far less than 65535. */
    uint16_t *freq = (uint16_t *)calloc((size_t)limit + 1, sizeof(uint16_t));
    if (!freq)
        return false;                 /* allocation failed */

    /* Enumerate all pairs (a,b) with 1 <= a <= b and a^2 + b^2 <= limit. */
    for (idx_t a = 1; ; ++a) {
        uint64_t aa = (uint64_t)a * (uint64_t)a;
        if (aa > limit)
            break;
        for (idx_t b = a; ; ++b) {
            uint64_t bb = (uint64_t)b * (uint64_t)b;
            uint64_t sum = aa + bb;
            if (sum > limit)
                break;
            ++freq[(size_t)sum];
        }
    }

    /* Collect numbers with exactly two representations. */
    term_cap = 4096;
    term_list = (seq_t *)malloc(term_cap * sizeof(seq_t));
    if (!term_list) {
        free(freq);
        return false;
    }

    term_len = 0;
    for (size_t v = 1; v <= limit; ++v) {
        if (freq[v] == 2) {
            if (term_len == term_cap) {
                term_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(term_list, term_cap * sizeof(seq_t));
                if (!tmp) {
                    free(freq);
                    free(term_list);
                    term_list = NULL;
                    term_cap = 0;
                    return false;
                }
                term_list = tmp;
            }
            term_list[term_len++] = (seq_t)v;   /* cast to seq_t */
        }
    }

    free(freq);
    initialized   = true;
    current_limit = limit;
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1, guard anyway */
        return 0;

    if (!initialized) {
        if (!build_table(DEFAULT_LIMIT))
            return 0;      /* allocation failed */
    }

    /* If we don't have enough terms yet, extend the limit until we do. */
    while (term_len < n) {
        idx_t new_limit = current_limit * 2;
        clear_tables();
        if (!build_table(new_limit))
            return 0;
    }

    return term_list[n - 1];
}
