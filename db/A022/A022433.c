/*
 * A022433  a(n) = c(n-1) + c(n-3) where c is the sequence of numbers not in
 *          a.
 * https://oeis.org/A022433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c(n-1) + c(n-3), where c is the sequence of numbers not
 *            in a. Initial values: a(0)=1, a(1)=3, a(2)=5. The sequence c is
 *            the complement of a
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Static cache variables */
static seq_t *a_vals = NULL;           /* array of a(0..a_len-1)          */
static size_t a_cap   = 0;
static size_t a_len   = 0;

static seq_t *c_vals = NULL;           /* array of c(0..c_len-1)          */
static size_t c_cap   = 0;
static size_t c_len   = 0;

static uint8_t *used_bits = NULL;      /* bitset of numbers used in a     */
static size_t used_bits_cap = 0;       /* number of bits allocated        */

static idx_t current_check = 1;        /* next integer to test for c     */

/* Ensure the array `a_vals` has capacity for `need` elements. */
static void ensure_a_capacity(size_t need)
{
    if (need <= a_cap) return;
    size_t new_cap = a_cap ? a_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    a_vals = tmp;
    a_cap   = new_cap;
}

/* Ensure the array `c_vals` has capacity for `need` elements. */
static void ensure_c_capacity(size_t need)
{
    if (need <= c_cap) return;
    size_t new_cap = c_cap ? c_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(c_vals, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    c_vals = tmp;
    c_cap   = new_cap;
}

/* Ensure the bitset `used_bits` can represent numbers up to `val`. */
static void ensure_used_capacity(idx_t val)
{
    size_t bits_needed = (size_t)val + 1;
    if (bits_needed <= used_bits_cap) return;
    size_t new_bits = used_bits_cap ? used_bits_cap * 2 : 16;
    while (new_bits < bits_needed) new_bits *= 2;
    size_t new_bytes = (new_bits + 7) / 8;
    uint8_t *tmp = (uint8_t *)realloc(used_bits, new_bytes);
    if (!tmp) exit(1);
    /* zero the newly allocated bytes */
    if (new_bytes > (used_bits_cap + 7) / 8)
        memset(tmp + (used_bits_cap + 7) / 8, 0,
               new_bytes - (used_bits_cap + 7) / 8);
    used_bits   = tmp;
    used_bits_cap = new_bits;
}

/* Mark number `val` as used in the sequence `a`. */
static void mark_used(idx_t val)
{
    ensure_used_capacity(val);
    used_bits[val >> 3] |= (uint8_t)(1 << (val & 7));
}

/* Test whether number `val` has been used (is in `a`). */
static bool is_used(idx_t val)
{
    if ((size_t)val >= used_bits_cap) return false;
    return (used_bits[val >> 3] >> (val & 7)) & 0x1;
}

/* Produce the next element of the complement sequence `c`. */
static void produce_next_c(void)
{
    /* Advance current_check until we find a number not used in `a`. */
    while (1) {
        if (!is_used(current_check)) {
            ensure_c_capacity(c_len + 1);
            c_vals[c_len++] = (seq_t)current_check;
            ++current_check;        /* move to next candidate for next call */
            return;
        }
        ++current_check;
    }
}

/* Ensure that `c_vals` contains at least `need` elements (0‑based). */
static void ensure_c_size(idx_t need)
{
    while (c_len <= (size_t)need) produce_next_c();
}

/* Initialization of the static cache on first call. */
static void init_cache(void)
{
    if (a_vals != NULL) return;            /* already initialized */

    /* Allocate initial space */
    ensure_a_capacity(16);
    ensure_c_capacity(16);
    ensure_used_capacity(16);

    /* Seed with known values */
    a_vals[0] = (seq_t)1; mark_used((idx_t)1);
    a_vals[1] = (seq_t)3; mark_used((idx_t)3);
    a_vals[2] = (seq_t)5; mark_used((idx_t)5);
    a_len = 3;

    /* Prepare first few c values needed for a(3) */
    ensure_c_size(2);   /* need c[0], c[1], c[2] */
}

/* Ensure `a_vals` contains values up to index `n`. */
static void ensure_a_size(idx_t n)
{
    init_cache();
    if (a_len > (size_t)n) return;

    for (idx_t i = a_len; i <= n; ++i) {
        if (i < 3) continue;        /* initial elements already present */

        /* Ensure enough c values are available */
        ensure_c_size(i - 1);

        /* a(i) = c(i-1) + c(i-3) */
        seq_t val = c_vals[i - 1] + c_vals[i - 3];

        /* Append to a and mark as used */
        ensure_a_capacity(a_len + 1);
        a_vals[a_len++] = val;
        mark_used((idx_t)val);
    }
}

seq_t a(idx_t n)
{
    ensure_a_size(n);
    return a_vals[n];
}
