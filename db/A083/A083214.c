/*
 * A083214  Numbers k for which 3 | p(k), where p(k) = A000041(k) is the k-th
 *          partition number.
 * https://oeis.org/A083214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence: k such that 3 divides p(k), where p(k) is the k‑th partition number.
 * This implementation keeps a running array of p(k) modulo 3 and returns the
 * k‑th element of the sequence in O(n) time, with memoisation. */

/* Global state for partition numbers modulo 3. */
static int   *part_mod = NULL;
static size_t part_cap = 0;
static idx_t  part_max_k = 0;   /* largest k whose p(k) % 3 is known */

/* Global state for the a-index sequence. */
static seq_t *a_vals = NULL;
static size_t a_cap   = 0;   /* allocated size of a_vals */
static size_t a_len   = 0;   /* number of a(·) values stored */

/* Ensure that part_mod can hold indices up to 'up_to' (inclusive). */
static void ensure_part_cap(size_t up_to)
{
    if (up_to < part_cap) return;
    size_t new_cap = part_cap < 64 ? 64 : part_cap * 2;
    while (new_cap <= up_to) new_cap *= 2;
    int *tmp = (int *)realloc(part_mod, new_cap * sizeof(int));
    if (!tmp) exit(1);
    part_mod = tmp;
    part_cap = new_cap;
}

/* Ensure that a_vals can hold 'len' elements. */
static void ensure_a_cap(size_t len)
{
    if (len <= a_cap) return;
    size_t new_cap = a_cap < 16 ? 16 : a_cap * 2;
    while (new_cap < len) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    a_vals = tmp;
    a_cap  = new_cap;
}

/* Compute partition numbers modulo 3 up to 'k' (inclusive). */
static void ensure_part_to(idx_t k)
{
    if (k <= part_max_k) return;
    ensure_part_cap((size_t)k + 1);

    /* Make sure p(0)=1 is defined. */
    if (part_max_k == 0) part_mod[0] = 1;

    for (idx_t n = part_max_k + 1; n <= k; ++n) {
        int sum = 0;
        for (int j = 1; ; ++j) {
            idx_t pent1 = (idx_t)j * (3 * j - 1) / 2;
            if (pent1 > n) break;
            int sign = (j & 1) ? 1 : -1;
            sum += sign * part_mod[(size_t)(n - pent1)];

            idx_t pent2 = (idx_t)j * (3 * j + 1) / 2;
            if (pent2 > n) continue;
            sum += sign * part_mod[(size_t)(n - pent2)];
        }
        int mod = sum % 3;
        if (mod < 0) mod += 3;
        part_mod[(size_t)n] = mod;
    }
    part_max_k = k;
}

/* Generate a(n) values until we have at least 'n' of them. */
static void ensure_a_terms(idx_t n)
{
    if (a_len >= n) return;

    if (a_len == 0) {
        ensure_a_cap(16);
        a_vals = (seq_t *)calloc(16, sizeof(seq_t));
        if (!a_vals) exit(1);
    }

    idx_t cur_k = part_max_k;  /* start searching from the last known k */
    while (a_len < n) {
        ++cur_k;
        ensure_part_to(cur_k);

        if (part_mod[(size_t)cur_k] == 0) { /* 3 divides p(cur_k) */
            if (a_len == a_cap) ensure_a_cap(a_len + 16);
            a_vals[a_len++] = (seq_t)cur_k;
        }
    }
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined, but guard anyway */
    ensure_a_terms(n);
    return a_vals[n - 1];
}
