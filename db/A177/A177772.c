/*
 * A177772  Base 10 numbers d_1 d_2 ... d_k such that the digits d_i are
 *          distinct, and Sum_{i=1..k-1} d_i^i = d_k^k.
 * https://oeis.org/A177772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: all base‑10 integers d1 d2 … dk with distinct digits such that  
   sum_{i=1}^{k-1} di^i = dk^k.  The sequence is sorted increasingly. */
/* Function: a(idx_t n) returns the nth term (1‑based).  Results are memoized. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

/* ----- utilities for comparison ------------------------------------ */
static inline int compare_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a, *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* ----- powers of digits -------------------------------------------- */
static unsigned long long pow_tbl_ul[10][11];
static bool pow_tbl_inited = false;

static inline void init_pow_tbl(void)
{
    if (pow_tbl_inited) return;
    for (int d = 0; d <= 9; ++d) {
        pow_tbl_ul[d][0] = 1ULL;          /* unused, but defined */
        for (int e = 1; e <= 10; ++e) {
            pow_tbl_ul[d][e] = pow_tbl_ul[d][e - 1] * (unsigned long long)d;
        }
    }
    pow_tbl_inited = true;
}

/* ----- maximum representable value of seq_t ------------------------ */
static unsigned long long max_seq_val;
static bool max_seq_init = false;

static inline void init_max_seq_val(void)
{
    if (max_seq_init) return;
    if (sizeof(seq_t) == 1)      max_seq_val = 0xFFULL;
    else if (sizeof(seq_t) == 2) max_seq_val = 0xFFFFULL;
    else if (sizeof(seq_t) == 4) max_seq_val = 0xFFFFFFFFULL;
    else if (sizeof(seq_t) == 8) max_seq_val = ULLONG_MAX;
    else                         max_seq_val = ULLONG_MAX; /* for larger types */
    max_seq_init = true;
}

/* ----- dynamic array for found numbers ----------------------------- */
typedef struct {
    seq_t *data;
    size_t len;
    size_t cap;
} seq_vec;

static seq_vec numbers = {NULL, 0, 0};

static void ensure_capacity(seq_vec *v, size_t needed)
{
    if (v->cap >= needed) return;
    size_t nc = v->cap ? v->cap * 2 : 8;
    if (nc < needed) nc = needed;
    seq_t *tmp = (seq_t *)malloc(nc * sizeof(seq_t));
    if (v->data) {
        memcpy(tmp, v->data, v->len * sizeof(seq_t));
        free(v->data);
    }
    v->data = tmp;
    v->cap  = nc;
}

static void push_seq(seq_vec *v, seq_t val)
{
    ensure_capacity(v, v->len + 1);
    v->data[v->len++] = val;
}

/* ----- recursive search -------------------------------------------- */
static void dfs_prefix(int k, int depth, int used_mask, int *digits,
                       unsigned long long sum)
{
    if (depth == k - 1) {
        /* choose final digit */
        for (int d = 0; d <= 9; ++d) {
            if (used_mask & (1 << d)) continue;
            unsigned long long last_pow = pow_tbl_ul[d][k];
            if (last_pow != sum) continue;

            /* build the number */
            unsigned long long num64 = 0;
            for (int i = 0; i < k - 1; ++i) {
                num64 = num64 * 10ULL + (unsigned long long)digits[i];
            }
            num64 = num64 * 10ULL + (unsigned long long)d;

            init_max_seq_val();
            if (sizeof(seq_t) < 8 && num64 > max_seq_val) continue;
            push_seq(&numbers, (seq_t)num64);
        }
        return;
    }

    for (int d = 0; d <= 9; ++d) {
        if (depth == 0 && d == 0) continue;          /* no leading zero */
        if (used_mask & (1 << d)) continue;          /* distinctness */
        digits[depth] = d;
        unsigned long long new_sum = sum + pow_tbl_ul[d][depth + 1];
        dfs_prefix(k, depth + 1, used_mask | (1 << d), digits, new_sum);
    }
}

/* ----- generate all numbers up to length 10 ----------------------- */
static void generate_all(void)
{
    init_pow_tbl();

    int digits[10];
    for (int k = 2; k <= 10; ++k) {
        dfs_prefix(k, 0, 0, digits, 0ULL);
    }

    /* sort numbers in ascending order */
    if (numbers.len > 1) {
        qsort(numbers.data, numbers.len, sizeof(seq_t), compare_seq_t);
    }
}

/* ----- public sequence function ------------------------------------ */
seq_t a(idx_t n)
{
    /* lazy evaluation */
    static bool initialized = false;
    if (!initialized) {
        generate_all();
        initialized = true;
    }

    if (n == 0 || n > numbers.len) return 0;
    return numbers.data[n - 1];
}
