/*
 * A141634  Nonprimes of the form c(c(c(c(c(i)-3)-3)-3)-3)-3, where
 *          A002808(i)=c(i)=i-th composite.
 * https://oeis.org/A141634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A141634
 * Nonprimes of the form c(c(c(c(c(i)-3)-3)-3)-3)-3,
 * where c(k) is the k‑th composite.
 * The function iterates over i = 1,2,…, evaluating the expression
 * and collects the resulting non‑prime numbers in order.
 * Composite numbers are generated on demand by enlarging a sieve
 * until it contains at least the required number of composites.
 * The function is memoised: previously found values are cached, so
 * repeated calls for any order of n are fast.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* ---- state for composites ------------------------------------------------ */
static bool      composites_inited = false;
static uint8_t   *is_prime_arr     = NULL;   /* 0 = not prime, 1 = prime */
static uint32_t *composite_arr    = NULL;   /* 1‑based: composite[1] = 4  */
static size_t     comp_count      = 0;       /* number of composites stored */
static uint64_t   limit_cur       = 0;       /* sieve limit that was used */

/* ---- state for sequence values ------------------------------------------- */
static seq_t  *seq_vals   = NULL;
static size_t  seq_cap    = 0;
static size_t  seq_cnt    = 0;   /* number of terms stored */
static idx_t   next_i     = 1;   /* next i to evaluate */

/* ---- helpers ------------------------------------------------------------- */
static void free_composite_arrays(void)
{
    free(is_prime_arr);
    free(composite_arr);
    is_prime_arr = NULL;
    composite_arr = NULL;
    comp_count = 0;
    limit_cur  = 0;
}

/* re‑create the sieve and composite list up to |limit| */
static void rebuild_composites(uint64_t limit)
{
    free_composite_arrays();
    limit_cur = limit;
    /* allocate prime sieve */
    is_prime_arr = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    if (!is_prime_arr) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= limit; ++i) is_prime_arr[i] = 1;
    is_prime_arr[0] = is_prime_arr[1] = 0;
    for (idx_t p = 2; (idx_t)p * p <= limit; ++p) {
        if (is_prime_arr[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                is_prime_arr[m] = 0;
        }
    }
    /* collect composites */
    composite_arr = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!composite_arr) exit(EXIT_FAILURE);
    comp_count = 0;
    for (uint32_t num = 4; num <= limit; ++num) {
        if (!is_prime_arr[num]) composite_arr[++comp_count] = num;
    }
}

/* ensure we have at least |needed| composites, extending the sieve if required */
static void ensure_composites_count(size_t needed)
{
    if (needed <= comp_count) return;

    if (!composites_inited) {
        rebuild_composites(64);   /* small start */
        composites_inited = true;
    }

    /* increase limit until comp_count >= needed */
    uint64_t new_limit = limit_cur;
    if (new_limit == 0) new_limit = 64;
    while (true) {
        rebuild_composites(new_limit);
        if (comp_count >= needed) break;
        new_limit <<= 1;                 /* double */
    }
}

/* make seq_vals large enough for up to |new_cap| entries */
static void ensure_seq_capacity(size_t new_cap)
{
    if (seq_cap >= new_cap) return;
    size_t cap = seq_cap ? seq_cap * 2 : 256;
    while (cap < new_cap) cap <<= 1;
    seq_vals = (seq_t *)realloc(seq_vals, cap * sizeof(seq_t));
    if (!seq_vals) exit(EXIT_FAILURE);
    seq_cap = cap;
}

/* extend the sequence until we have at least n terms */
static void extend_sequence_up_to(size_t n)
{
    while (seq_cnt < n) {
        idx_t val = next_i;   /* start with i */
        for (int step = 0; step < 5; ++step) {
            /* we need the val‑th composite number */
            if ((size_t)val > comp_count) {
                ensure_composites_count((size_t)val);
            }
            val = (idx_t)composite_arr[val] - 3;
        }
        seq_t v = (seq_t)val;
        /* keep it only if it is not prime */
        if (!is_prime_big(v)) {
            ensure_seq_capacity(seq_cnt + 1);
            seq_vals[seq_cnt++] = v;
        }
        ++next_i;
    }
}

/* ---- required public function -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* out of bounds */
    extend_sequence_up_to((size_t)n);
    return seq_vals[n - 1];
}
