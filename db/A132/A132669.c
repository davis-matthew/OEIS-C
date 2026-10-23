/*
 * A132669  a(1)=1, a(n) = 5*a(n-1) if the minimal positive integer not yet in
 *          the sequence is greater than a(n-1), else a(n) = a(n-1) - 1.
 * https://oeis.org/A132669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A132669
   Recurrence:
     a(1) = 1;
     for n > 1 let m be the minimal positive integer not yet in the sequence.
     if m > a(n‑1) then a(n) = 5 * a(n‑1);
     else              a(n) = a(n‑1) - 1.
   The function below generates the sequence lazily, remembering all
   values computed so far so it can answer calls in any order.  Used
   numbers are recorded in a boolean array whose size is always at least
   seq_len + 1 – which is enough because the smallest missing integer
   can never exceed the number of terms seen plus one.  This keeps the
   memory footprint small and works for all supported seq_t types.
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static seq_t *seq_arr = NULL;      /* sequence values, 1‑based */
static size_t seq_cap = 0;
static size_t seq_len = 0;

static bool *used_arr = NULL;      /* flags for used integers */
static size_t used_cap = 0;
static size_t next_missing = 2;    /* minimal positive integer not yet used */

/* enlarge seq_arr if needed */
static void ensure_seq_capacity(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 256;
    while (new_cap < needed) new_cap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* enlarge used_arr if needed */
static void ensure_used_capacity(size_t needed)
{
    if (used_cap >= needed) return;
    size_t new_cap = used_cap ? used_cap << 1 : 256;
    while (new_cap < needed) new_cap <<= 1;
    bool *tmp = (bool *)realloc(used_arr, new_cap * sizeof(bool));
    /* initialise new slots to false */
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used_arr = tmp;
    used_cap = new_cap;
}

/* initialise the first term */
static void init_sequence(void)
{
    if (seq_len > 0) return;          /* already initialized */

    seq_cap = 256;
    seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));

    used_cap = 256;
    used_arr = (bool *)malloc(used_cap * sizeof(bool));
    memset(used_arr, 0, used_cap * sizeof(bool));

    seq_len = 1;
    seq_arr[1] = (seq_t)1;
    used_arr[1] = true;
    next_missing = 2;
}

/* public function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n < 1 */

    init_sequence();

    size_t N = (size_t)n;
    while (seq_len < N) {
        seq_t prev = seq_arr[seq_len];
        seq_t next_val = (next_missing > prev) ?
                          prev * (seq_t)5 : prev - (seq_t)1;

        /* store next value */
        seq_len++;
        ensure_seq_capacity(seq_len + 1);
        seq_arr[seq_len] = next_val;

        /* mark it as used if it lies within our current bool array */
        size_t idx = (size_t)next_val;
        if (idx <= used_cap) {
            used_arr[idx] = true;
        }

        /* update smallest missing integer */
        ensure_used_capacity(next_missing);
        while (next_missing <= used_cap && used_arr[next_missing]) {
            ++next_missing;
        }
    }

    return seq_arr[N];
}
