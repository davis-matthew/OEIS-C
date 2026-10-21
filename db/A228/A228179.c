/*
 * A228179  Irregular table where the n-th row consists of the square roots of
 *          1 in Z_n.
 * https://oeis.org/A228179
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is the n-th entry of the flattened table formed by *
 *            concatenating, for each m = 2, 3, 4, …, the set of all integers
 *            * x (0 < x < m) satisfying x² ≡ 1 (mod m). The offset is 2, so *
 *            a(2) is the first entry, which is the sole solution for m = 2
 * Range:     u8 n <= 1011, u16 n <= 9686, u32 n <= 9686, u64 n <= 9686,
 *            u128 n <= 9686, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* External types supplied by the build */
typedef uint64_t idx_t;          /* indices are 64‑bit unsigned integers */

static seq_t *table = NULL;      /* cached flat sequence */
static size_t table_len = 0;     /* number of elements stored */
static size_t table_cap = 0;     /* allocated capacity */
static idx_t next_m = 2;         /* next modulus to process */

/* Grow the cache when needed */
static void grow_cache(void)
{
    size_t new_cap = table_cap ? table_cap * 2 : 16;
    seq_t *new_buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_buf) abort();   /* out of memory */
    memcpy(new_buf, table, table_len * sizeof(seq_t));
    free(table);
    table = new_buf;
    table_cap = new_cap;
}

/* Append a single value to the cache */
static void append_to_cache(seq_t val)
{
    if (table_len == table_cap) grow_cache();
    table[table_len++] = val;
}

/* Generate all x (0 < x < m) with x^2 ≡ 1 (mod m) and append them */
static void generate_row(idx_t m)
{
    for (idx_t x = 1; x < m; ++x) {
        uint64_t prod = (uint64_t)x * (uint64_t)x;
        if ((prod % m) == 1) {
            append_to_cache((seq_t)x);
        }
    }
}

/* Ensure that the cache contains a(n) for all n up to the requested index */
static void ensure_cache(idx_t n)
{
    if (n < 2) return;               /* values below offset are undefined */
    while (table_len <= (size_t)(n - 2)) {
        generate_row(next_m);
        ++next_m;
    }
}

/* --------------  Main sequence function -------------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;              /* unspecified below offset */
    ensure_cache(n);
    return table[n - 2];
}
