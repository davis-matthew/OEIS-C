/*
 * A065056  Partial sums of Recamán's sequence A005132.
 * https://oeis.org/A065056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 285, u32 n <= 9507, u64 n <= 9507,
 *            u128 n <= 9507, big any n
 * Helpers:   to_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial sums of Recamán's sequence A005132
   Formula: a(n) = Σ_{k=0..n} r(k),   r(0)=0, 
            r(k)=r(k-1)-k if positive and unseen, else r(k)=r(k-1)+k
   The function memoises all partial sums and the set of seen Recamán
   values, so repeated calls with arbitrary n are answered in O(n).
   Recamán values for n ≤ 10 000 comfortably fit in a 64‑bit integer,
   so the bitset of seen values uses an idx_t (uint64_t) index.
   For builds where seq_t is an arbitrary‑precision integer class,
   conversions are performed via C‑style casts. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/to_idx.h"

static seq_t *sum_arr = NULL;          /* sum_arr[0] = 0, sum_arr[i] = a(i) */
static size_t sum_cap = 0;             /* allocated capacity of sum_arr */
static idx_t sum_len = 0;              /* number of stored sums (sum_len = last_index+1) */

static unsigned char *used_bits = NULL;/* bitset of values already seen in Recamán */
static size_t used_bytes = 0;          /* number of bytes currently allocated */

static seq_t current_rec = 0;          /* current Recamán value r(i-1) */

/* test whether value < val has been visited already */
static inline bool is_used(seq_t val)
{
    idx_t v = to_idx(val);
    size_t byte_index = v >> 3;
    if (used_bytes <= byte_index) return false;
    return !!(used_bits[byte_index] & (1u << (v & 7u)));
}

/* mark a value as visited (expanding bitset if necessary) */
static inline void mark_used(seq_t val)
{
    idx_t v = to_idx(val);
    size_t byte_index = v >> 3;
    if (used_bytes <= byte_index) {
        size_t new_bytes = byte_index + 1;
        unsigned char *tmp = (unsigned char*)realloc(used_bits, new_bytes);
        if (!tmp) exit(1);                 /* memory allocation failed */
        memset(tmp + used_bytes, 0, new_bytes - used_bytes);
        used_bits = tmp;
        used_bytes = new_bytes;
    }
    used_bits[byte_index] |= (1u << (v & 7u));
}

/* ensure sum_arr has room for indices up to idx (inclusive) */
static void ensure_sum_cap(idx_t idx)
{
    while (idx >= (idx_t)sum_cap) {
        sum_cap = sum_cap ? sum_cap * 2 : 16;
        seq_t *tmp = (seq_t*)realloc(sum_arr, sum_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        sum_arr = tmp;
    }
}

/* initialization of static data */
static void init_static(void)
{
    if (sum_arr == NULL) {
        sum_cap = 16;
        sum_arr = (seq_t*)malloc(sum_cap * sizeof(seq_t));
        if (!sum_arr) exit(1);
        sum_arr[0] = (seq_t)0;
        sum_len = 1;                      /* sum_arr[0] already defined */

        current_rec = (seq_t)0;

        used_bytes = 1;
        used_bits = (unsigned char*)malloc(used_bytes);
        if (!used_bits) exit(1);
        memset(used_bits, 0, used_bytes);
        mark_used((seq_t)0);
    }
}

seq_t a(idx_t n)
{
    init_static();

    if (n >= sum_len) {
        for (idx_t i = sum_len; i <= n; ++i) {
            idx_t step = i;
            /* try r_{i-1} - step */
            seq_t cand = current_rec - (seq_t)step;
            bool pos = current_rec > (seq_t)step;
            seq_t new_rec;
            if (pos && !is_used(cand))
                new_rec = cand;
            else
                new_rec = current_rec + (seq_t)step;

            current_rec = new_rec;
            mark_used((seq_t)new_rec);

            ensure_sum_cap(i);
            sum_arr[i] = sum_arr[i - 1] + new_rec;
        }
        sum_len = n + 1;
    }

    return sum_arr[n];
}
