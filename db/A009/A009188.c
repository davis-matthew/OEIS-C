/*
 * A009188  Short leg of more than one Pythagorean triangle.
 * https://oeis.org/A009188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer that is the short leg of more than one
 *            Pythagorean triple. The sequence is OEIS A009188 (offset 1)
 * Range:     u8 n <= 168, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <new>    /* needed for placement new */
#endif

/* index type */
typedef uint64_t idx_t;
/* sequence type – provided by the build; may be an integer or an
   arbitrary‑precision integer class.  The code below works for both. */

/* Euclidean gcd for uint64_t */
static inline idx_t gcd_uint64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* destroy an array of seq_t objects (only needed in C++). */
#ifdef __cplusplus
static inline void destroy_seq_array(seq_t *arr, size_t sz)
{
    for (size_t i = 0; i < sz; ++i)
        arr[i].~seq_t();
}
#else
static inline void destroy_seq_array(seq_t *arr, size_t sz) { (void)arr; (void)sz; }
#endif

/* Global state shared by calls. */
static seq_t *values   = NULL;  /* array of terms (allocated with malloc) */
static size_t val_count = 0;     /* number of stored terms   */
static uint64_t processed_limit = 0;   /* last limit processed     */

/* Build the list of terms up to the given short‑leg limit. */
static void generate_up_to(uint64_t limit)
{
    /* Count how many times each short leg occurs. */
    uint16_t *cnt = (uint16_t *)calloc((size_t)(limit + 1), sizeof(uint16_t));
    if (!cnt) abort();

    /* Upper bound for m: 2*m*n ≤ limit for n = 1  →  m ≤ limit/2. */
    uint64_t m_max = (limit >> 1) + 1;

    for (uint64_t m = 2; m <= m_max; ++m) {
        for (uint64_t n = 1; n < m; ++n) {
            if (((m - n) & 1) == 0)            /* same parity → skip    */
                continue;
            if (gcd_uint64(m, n) != 1)         /* not coprime → skip   */
                continue;

            uint64_t a = m * m - n * n;
            uint64_t b = 2ULL * m * n;
            uint64_t s = a < b ? a : b;         /* short leg */

            if (s == 0 || s > limit)
                continue;

            /* Count all multiples of s up to the limit. */
            uint64_t max_k = limit / s;
            for (uint64_t k = 1; k <= max_k; ++k)
                ++cnt[k * s];
        }
    }

    /* Count how many numbers qualify. */
    size_t pos = 0;
    for (uint64_t s = 1; s <= limit; ++s)
        if (cnt[s] >= 2)
            ++pos;

    /* Allocate array for the qualifying numbers. */
    seq_t *new_vals = NULL;
    if (pos > 0) {
        new_vals = (seq_t *)malloc(pos * sizeof(seq_t));
        if (!new_vals) abort();
        size_t idx = 0;
        for (uint64_t s = 1; s <= limit; ++s) {
            if (cnt[s] >= 2) {
#ifdef __cplusplus
                /* placement new constructs the object with value s. */
                new(&new_vals[idx]) seq_t((seq_t)s);
#else
                new_vals[idx] = (seq_t)s;
#endif
                ++idx;
            }
        }
    }

    free(cnt);

    /* Replace old list with the new one. */
    destroy_seq_array(values, val_count);
    free(values);
    values = new_vals;
    val_count = pos;
    processed_limit = limit;
}

/* Ensure that at least 'needed' terms have been generated. */
static void ensure_terms(uint64_t needed)
{
    if (val_count >= needed)
        return;

    uint64_t limit = processed_limit ? processed_limit : 128;
    while (val_count < needed) {
        generate_up_to(limit);
        if (val_count >= needed)
            break;
        limit <<= 1; /* double the bound */
    }
}

/* External function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* safety for 0‑based callers */
    ensure_terms((uint64_t)n);
    return values[(size_t)(n - 1)];
}
