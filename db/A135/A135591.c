/*
 * A135591  a(1)=1; for n > 1, a(n) is number of earlier terms equal to number
 *          of proper divisors of n.
 * https://oeis.org/A135591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 838, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A135591:
   a(1) = 1
   For n > 1, a(n) is the number of earlier terms equal to the number of proper divisors of n.
   The sequence is memoised so any order of calls works.
*/
#include <stddef.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"  /* divisor_count_u64(uint64_t) -> unsigned long long */
#include "../utils/grow_seq.h"          /* void grow_seq(seq_t **arr, size_t *cap, size_t need) */

static seq_t *a_vals = NULL;   /* a_vals[i] = a(i+1), 0‑based */
static size_t a_len  = 0;      /* number of terms currently stored */
static size_t a_cap  = 0;      /* capacity of a_vals */

static seq_t *freq   = NULL;   /* freq[v] = number of computed a(k) equal to v */
static size_t freq_cap = 0;    /* capacity of freq */

static void ensure_a_capacity(size_t need)
{
    if (a_cap >= need) return;
    grow_seq(&a_vals, &a_cap, need);
}

static void ensure_freq_capacity(size_t need)
{
    if (freq_cap >= need) return;
    size_t old_cap = freq_cap;
    grow_seq(&freq, &freq_cap, need);
    /* zero new entries */
    for (size_t i = old_cap; i < freq_cap; ++i)
        freq[i] = 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1, harmless */

    size_t target = (size_t)n;

    /* Initialise first term if necessary */
    if (a_len == 0) {
        ensure_a_capacity(1);
        a_vals[0] = (seq_t)1;          /* a(1) = 1 */
        a_len  = 1;
        ensure_freq_capacity(2);       /* need index 1 */
        freq[1] = (seq_t)1;            /* one occurrence of value 1 */
    }

    /* Extend the sequence up to the requested index */
    for (size_t i = a_len + 1; i <= target; ++i) {
        uint64_t idx = (uint64_t)i;

        /* Number of proper divisors = τ(i) - 1 */
        seq_t total_div = divisor_count_u64(idx);
        seq_t proper_div = total_div - (seq_t)1;

        size_t pdx = (size_t)proper_div;   /* proper_div is small */
        seq_t count = 0;
        if (pdx < freq_cap) count = freq[pdx];

        /* Store new term */
        ensure_a_capacity(i);
        a_vals[i - 1] = count;

        /* Update frequency table for the new value */
        size_t cidx = (size_t)count;
        ensure_freq_capacity(cidx + 1);
        freq[cidx] = freq[cidx] + (seq_t)1;
    }

    a_len = target;
    return a_vals[n - 1];
}
