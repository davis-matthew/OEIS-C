/*
 * A201056  Composite numbers whose product of digits is 8.
 * https://oeis.org/A201056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   compute by generating composite numbers whose product of digits
 *            equals 8, using a recursive search for each digit length
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static int seq_cmp(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

static const int allowed_digits[] = {1, 2, 4, 8};
static const size_t NUM_DIGITS = sizeof(allowed_digits) / sizeof(allowed_digits[0]);

/* Recursive helper: build all numbers of a given length that satisfy
   the remaining digit product (remProd). */
static void gen_rec(int len, int pos, int remProd, seq_t cur,
                    seq_t **seq, size_t *len_ptr, size_t *cap_ptr)
{
    if (pos == len) {
        if (remProd == 1) {
            if (!is_prime_big(cur))
                append_candidate(seq, len_ptr, cap_ptr, cur);
        }
        return;
    }

    /* If remaining product is 1, all remaining positions must be 1's. */
    if (remProd == 1) {
        seq_t val = cur;
        for (int i = pos; i < len; ++i)
            val = val * 10 + 1;
        if (!is_prime_big(val))
            append_candidate(seq, len_ptr, cap_ptr, val);
        return;
    }

    for (size_t i = 0; i < NUM_DIGITS; ++i) {
        int d = allowed_digits[i];
        if (remProd % d != 0)
            continue;
        seq_t next = cur * 10 + (seq_t)d;
        gen_rec(len, pos + 1, remProd / d, next, seq, len_ptr, cap_ptr);
    }
}

/* Generate all numbers of a specific length and append the composites. */
static void generate_length(int len, seq_t **seq, size_t *len_ptr, size_t *cap_ptr)
{
    gen_rec(len, 0, 8, 0, seq, len_ptr, cap_ptr);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static int max_len_generated = 0;

    /* Generate numbers until we have at least n terms. */
    while (seq_len < n) {
        ++max_len_generated;
        generate_length(max_len_generated, &seq, &seq_len, &seq_cap);
    }

    /* Sort the sequence once per augmentation. */
    mem_qsort(seq, seq_len, sizeof(seq_t), (int(*)(const void *, const void *))seq_cmp);

    return seq[n - 1];
}
