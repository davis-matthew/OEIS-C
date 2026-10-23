/*
 * A116626  a(1)=1; a(odd n) = a(n-1) XOR a(n-2), for a(even n) we find the
 *          first i > 1 such that neither i nor (i XOR A116626(n-1)) is
 *          present in A116626(1..n-1), in which case a(n) = (i XOR
 *          A116626(n-1)).
 * https://oeis.org/A116626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A116626:  a(1)=1;
 * for odd n > 1, a(n)=a(n‑1) XOR a(n‑2);
 * for even n, a(n) is the first i>1 such that neither i nor (i XOR a(n‑1))
 * has appeared among a(1) … a(n‑1);  then a(n)=i XOR a(n‑1).
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static seq_t *seq = NULL;          /* array of computed terms          */
static size_t seq_len   = 0;       /* number of terms stored           */
static size_t seq_cap   = 0;       /* capacity of seq array            */

static bool *present = NULL;      /* bitmap of values that have appeared */
static size_t present_cap = 0;     /* capacity of present array */

/* Ensure the seq array can hold 'min_cap' elements */
static void ensure_seq_capacity(size_t min_cap)
{
    if (seq_cap >= min_cap) return;
    size_t new_cap = seq_cap > 0 ? seq_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *new_arr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);
    seq = new_arr;
    seq_cap = new_cap;
}

/* Ensure the present array can index at least 'index' */
static void ensure_present_capacity(size_t index)
{
    if (present_cap > index) return;
    size_t new_cap = present_cap > 0 ? present_cap * 2 : 256;
    if (new_cap <= index) new_cap = index + 16;
    bool *new_arr = (bool *)realloc(present, new_cap * sizeof(bool));
    if (!new_arr) exit(EXIT_FAILURE);
    if (new_cap > present_cap)
        memset(new_arr + present_cap, 0, (new_cap - present_cap) * sizeof(bool));
    present = new_arr;
    present_cap = new_cap;
}

/* Record that 'value' has appeared */
static void mark_present(seq_t value)
{
    size_t idx = (size_t)value;
    ensure_present_capacity(idx);
    present[idx] = true;
}

/* Check whether 'value' has already appeared */
static bool is_present(seq_t value)
{
    size_t idx = (size_t)value;
    if (idx >= present_cap) return false;
    return present[idx];
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n<1, return 0 */

    /* Compute terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        size_t k = seq_len + 1;      /* compute a(k) */
        seq_t val;
        if (k == 1) {
            val = (seq_t)1;
        } else if (k % 2 == 1) { /* odd k > 1 */
            seq_t prev      = seq[seq_len - 1];
            seq_t before_prev = seq[seq_len - 2];
            val = prev ^ before_prev;
        } else { /* even k */
            seq_t prev = seq[seq_len - 1];
            seq_t i = 2;
            for (;;) {
                if (!is_present(i)) {
                    seq_t xor_val = i ^ prev;
                    if (!is_present(xor_val)) {
                        val = xor_val;
                        break;
                    }
                }
                i++;
            }
        }
        /* store the new term */
        ensure_seq_capacity(seq_len + 1);
        seq[seq_len] = val;
        mark_present(val);
        ++seq_len;
    }
    return seq[n-1];
}
