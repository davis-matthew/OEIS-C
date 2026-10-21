/*
 * A060877  Intrinsic 7-palindromes: n is an intrinsic k-palindrome if it is a
 *          k-digit palindrome in some base.
 * https://oeis.org/A060877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   A060877: intrinsic 7‑palindromes
 *
 *   a(n) = the n‑th smallest positive integer that can be written as
 *   a 7‑digit palindrome in some base b ≥ 2.
 *
 *   The implementation enumerates all 7‑digit palindromes for every base
 *   up to the current requirement, stores their values, sorts them and
 *   removes duplicates.  Calls to a(n) are answered in O(1) after a
 *   one‑time pre‑computation that covers the requested index.
 *   The algorithm stays well under the 2.0‑second limit for n up to ≈
 *   10 000.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- global storage ---------------------------------------------- */
static seq_t *global_values = NULL; /* sorted, unique list of values */
static size_t  global_count  = 0;    /* number of stored values          */
static size_t  global_cap    = 0;    /* capacity of allocated array      */
static uint64_t processed_base = 1;  /* last base that was processed    */

/* --- helpers ------------------------------------------------------ */
/* Compare two seq_t values for qsort. Works for built‑in ints and
 * arbitrary‑precision types supplied at compile time. */
static int seq_cmp(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Ensure that the global array can hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= global_cap) return;
    size_t new_cap = global_cap ? global_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(global_values, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* out of memory */
    global_values = tmp;
    global_cap    = new_cap;
}

/* Append a new value to the global array. */
static void append_value(seq_t val)
{
    ensure_capacity(global_count + 1);
    global_values[global_count++] = val;
}

/* Generate all 7‑digit palindromes for a given base and store their
 * integer values (in base 10).  The digits are
 * a b c d c b a with a≠0, 0≤b,c,d<b. */
static void generate_base(uint64_t base)
{
    /* powers of base: pow[k] = base^k for 0 ≤ k ≤ 6 */
    seq_t pow[7];
    pow[0] = 1;
    for (int k = 1; k <= 6; ++k)
        pow[k] = pow[k - 1] * (seq_t)base;

    /* Pre‑multiply the structural terms */
    seq_t term_a = pow[6] + 1;          /* a*(b^6+1)          */
    seq_t term_b = pow[5] + pow[1];     /* b*(b^5+b)          */
    seq_t term_c = pow[4] + pow[2];     /* c*(b^4+b^2)        */
    seq_t term_d = pow[3];              /* d*b^3              */

    for (uint64_t a = 1; a < base; ++a) {
        seq_t va = (seq_t)a * term_a;
        for (uint64_t b = 0; b < base; ++b) {
            seq_t vb = (seq_t)b * term_b;
            for (uint64_t c = 0; c < base; ++c) {
                seq_t vc = (seq_t)c * term_c;
                for (uint64_t d = 0; d < base; ++d) {
                    seq_t vd = (seq_t)d * term_d;
                    append_value(va + vb + vc + vd);
                }
            }
        }
    }
}

/* Remove duplicates from a sorted array in place. Return new length. */
static size_t dedup_sorted(seq_t *array, size_t len)
{
    if (len == 0) return 0;
    size_t new_len = 1;
    for (size_t i = 1; i < len; ++i) {
        if (seq_cmp(&array[i], &array[new_len - 1]) != 0)
            array[new_len++] = array[i];
    }
    return new_len;
}

/* Pre‑compute values for bases 2 … 20.  These bases generate >16 000
 * distinct values, more than enough for n up to ~10 000. */
static void initial_precompute(void)
{
    if (global_count != 0) return; /* already done */

    /* Rough estimate of how many values we will have up to base 20. */
    size_t est = 0;
    for (uint64_t b = 2; b <= 20; ++b)
        est += (b - 1) * b * b * b; /* number of 7‑digit palindromes in base b */

    ensure_capacity(est);
    global_cap = est;  /* force this capacity so that global_count is correct */

    for (uint64_t b = 2; b <= 20; ++b)
        generate_base(b);

    qsort(global_values, global_count, sizeof(seq_t), seq_cmp);
    global_count = dedup_sorted(global_values, global_count);
    processed_base = 20;
}

/* Extend the data set until it contains at least 'n' elements.
 * Bases are processed sequentially. */
static void extend_to(idx_t n)
{
    while ((size_t)n > global_count) {
        ++processed_base;
        generate_base(processed_base);
        /* Merge the new values into the sorted unique list. */
        qsort(global_values, global_count, sizeof(seq_t), seq_cmp);
        global_count = dedup_sorted(global_values, global_count);
    }
}

/* ------------------------------------------------------------------ */
/* The required function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for 0 (safe return) */

    initial_precompute();

    if ((size_t)n > global_count)
        extend_to(n);

    /* n is 1‑based; array is 0‑based. */
    return global_values[(size_t)n - 1];
}
