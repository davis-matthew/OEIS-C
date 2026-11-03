/*
 * A193458  Union of A071863 and A071861.
 * https://oeis.org/A193458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   is_prime_seq, mem, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A193458: Union of A071863 and A071861.
   A(n) = the n‑th integer that satisfies:
     (1)  sopfr(n) is a prime factor of n+1
     or
     (2)  sopfr(n+1) is a prime factor of n
   sopfr(x) is the sum of prime factors of x counted with multiplicity.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stddef.h>
#include "../utils/is_prime_seq.h"
#include "../utils/smallest_prime_factor.h"
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* local helper: sum of prime factors with multiplicity (uint64_t version) */
static uint64_t sopfr_uint64(uint64_t x)
{
    uint64_t sum = 0;
    while (x > 1) {
        uint64_t p = smallest_prime_factor(x);
        if (p == 0)    /* safety for 1 */
            p = x;
        sum += p;
        x /= p;
    }
    return sum;
}

/* static storage for memoized terms */
static seq_t *seq = NULL;            /* array of sequence values   */
static idx_t seq_len = 0;            /* number of values stored    */
static size_t seq_cap = 0;           /* allocated capacity         */
static uint64_t last_checked = 0;    /* largest integer examined   */

/* append a value, extending the array if necessary */
static void append_value(seq_t v)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 32;
        grow_seq(&seq, &seq_cap, new_cap);
    }
    seq[seq_len++] = v;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* initialise array on first call */
    if (!seq) {
        seq = alloc_seq_array(32);
        seq_cap = 32;
        seq_len = 0;
    }

    /* extend the sequence until we have at least n terms */
    while (seq_len < n) {
        ++last_checked;

        uint64_t sopfr_n   = sopfr_uint64(last_checked);
        uint64_t sopfr_np1 = sopfr_uint64(last_checked + 1);

        bool ok = false;
        if (sopfr_n > 1 && is_prime_seq((seq_t)sopfr_n) &&
            ((last_checked + 1) % sopfr_n == 0))
            ok = true;
        else if (sopfr_np1 > 1 && is_prime_seq((seq_t)sopfr_np1) &&
                 (last_checked % sopfr_np1 == 0))
            ok = true;

        if (ok)
            append_value((seq_t)last_checked);
    }

    return seq[n - 1];
}
