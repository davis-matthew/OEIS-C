/*
 * A065652  a(0) = 0 and a(n+1) = if a(n) - 1 is new and > 0 then a(n) - 1
 *          else a(n)*a(n) + 1 for n >= 0.
 * https://oeis.org/A065652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 122, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(0) = 0
 *   a(n+1) = if a(n) > 0 and a(n)-1 has NOT appeared before then
 *              a(n) - 1
 *            else
 *              a(n) * a(n) + 1
 * This implements OEIS sequence A065652.  The sequence is a
 * self‑inverse permutation of the naturals, but the recurrence
 * above suffices for all terms.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* --- static storage for memoisation ----------------------------------- */
static seq_t *seq = NULL;      /* array of already computed values      */
static size_t seq_capacity = 0;/* allocated size of seq                 */
static size_t seq_size = 0;     /* number of elements actually stored    */

/* Initialise the static state on first use */
static void init_static_state(void)
{
    if (seq == NULL) {
        seq_capacity = 16;
        seq = (seq_t *)malloc(seq_capacity * sizeof(seq_t));
        seq_size = 1;                  /* we already have a(0)            */
        seq[0] = (seq_t)0;             /* a(0) = 0                          */
    }
}

/* Grow the seq array so that it can store at least (need+1) elements    */
static void ensure_seq_cap(size_t need)
{
    while (seq_capacity <= need) {
        size_t new_cap = seq_capacity * 2;
        seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq = new_seq;
        seq_capacity = new_cap;
    }
}

/* Linear search: true if value is present in arr[0 .. last-1]        */
static int seen_contains(const seq_t *arr, size_t last, seq_t value)
{
    for (size_t i = 0; i < last; ++i)
        if (arr[i] == value)
            return 1;
    return 0;
}

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_static_state();

    size_t idx = (size_t)n;           /* safe: idx_t <= uint64_t */

    if (seq_size <= idx) {
        /* generate terms from seq_size up through idx (inclusive)       */
        ensure_seq_cap(idx);           /* capacity check                  */
        for (size_t i = seq_size; i <= idx; ++i) {
            seq_t prev = seq[i - 1];
            seq_t val;
            if (prev > (seq_t)0 && !seen_contains(seq, i, prev - (seq_t)1))
                val = prev - (seq_t)1;
            else
                val = prev * prev + (seq_t)1;
            seq[i] = val;
        }
        seq_size = idx + 1;            /* all indices 0..idx are present   */
    }

    return seq[idx];
}
