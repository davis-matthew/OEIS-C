/*
 * A057102  a(n) = 4 * A073120(n).
 * https://oeis.org/A057102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 4 * (x³·y – x·y³) for positive integers x > y, sorted in
 *            ascending order. (The first term corresponds to x=2, y=1.)
 * Range:     u8 none, u16 none, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Forward declaration of the comparison callback used by qsort. */
static int cmp_seq(const void *a, const void *b);

/* Static state for memoised sequence values. */
static seq_t *val_arr      = NULL;  /* dynamic array of sequence values */
static size_t val_cap      = 0;     /* allocated capacity of val_arr   */
static size_t val_len      = 0;     /* number of values currently stored */
static idx_t  proc_x      = 0;      /* largest x already processed    */
static idx_t  max_x       = 200;    /* current bound for x generation  */
static bool   initialized = false;

/* Append a value to the dynamic array, enlarging it if necessary. */
static void append_val(seq_t x)
{
    if (val_len == val_cap) {
        size_t new_cap = (val_cap == 0) ? 1024 : val_cap * 2;
        seq_t *tmp = (seq_t *)realloc(val_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure; terminate */
        val_arr = tmp;
        val_cap = new_cap;
    }
    val_arr[val_len++] = x;
}

/* Generate all values for x in (proc_x, max_x] and store them in val_arr. */
static void generate_up_to(idx_t upper)
{
    for (idx_t x = proc_x + 1; x <= upper; ++x) {
        seq_t sx = (seq_t)x;
        seq_t sx2 = sx * sx;          /* x² */
        for (idx_t y = 1; y < x; ++y) {   /* y < x to ensure positivity */
            seq_t sy = (seq_t)y;
            seq_t sy2 = sy * sy;          /* y² */
            /* (x³·y – x·y³) = x·y·(x² – y²) */
            seq_t part = sx * sy * (sx2 - sy2);
            /* a(n) is four times that */
            part *= (seq_t)4;
            append_val(part);
        }
    }
    proc_x = upper;
}

/* Sort the array, remove duplicates, leaving unique values in ascending order. */
static void sort_and_unique(void)
{
    if (val_len == 0) return;
    qsort(val_arr, val_len, sizeof(seq_t), cmp_seq);
    /* In‑place removal of duplicates. */
    size_t new_len = 1;
    for (size_t i = 1; i < val_len; ++i) {
        if (val_arr[i] != val_arr[new_len - 1]) {
            val_arr[new_len++] = val_arr[i];
        }
    }
    val_len = new_len;
}

/* Ensure that the array contains at least n terms of the sequence. */
static void ensure_n_terms(idx_t n)
{
    if (!initialized) {
        generate_up_to(max_x);
        sort_and_unique();
        initialized = true;
    }
    while ((idx_t)val_len < n) {
        max_x *= 2;
        generate_up_to(max_x);
        sort_and_unique();
    }
}

/* Comparator for seq_t used by qsort. Works for built‑in integer types and
   for arbitrary‑precision types that support operator<. */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    if (*pa < *pb) return -1;
    if (*pa > *pb) return 1;
    return 0;
}

/* Public sequence function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; value unused for n==0 */
    ensure_n_terms(n);
    return val_arr[n - 1];
}
