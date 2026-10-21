/*
 * A099257  a(1)=1, a(n+1) = if a(n)=n then 2*n+1 else smallest number not
 *          occurring earlier.
 * https://oeis.org/A099257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n≥1
   a(n+1) = (a(n)==n) ? 2*n+1 : smallest unused positive integer. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* --- static storage ----------------------------------------------------- */
static seq_t *seq_arr = NULL;          /* a(1)..a(k) stored (0‑based)       */
static size_t  seq_cap = 0;            /* capacity of seq_arr               */
static size_t  seq_len = 0;            /* number of terms already cached    */

static uint8_t *used = NULL;           /* bitmap of numbers already seen    */
static size_t   used_cap = 0;           /* capacity of used array            */
static idx_t    next_unused = 1;        /* next candidate for smallest unused */

/* --- ensure sequence array has space for at least 'need' terms --------- */
static void ensure_seq_capacity(idx_t need)
{
    if ((size_t)need <= seq_cap) return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < (size_t)need) new_cap <<= 1;

    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) return;                    /* allocation failure – abort */

    if (seq_arr) {
        memcpy(new_arr, seq_arr, seq_len * sizeof(seq_t));
        free(seq_arr);
    }
    seq_arr = new_arr;
    seq_cap = new_cap;
}

/* --- ensure bitmap can index at least 'need-1' --------------------------------- */
static void ensure_used_capacity(idx_t need)
{
    if ((size_t)need <= used_cap) return;

    size_t new_cap = used_cap ? used_cap * 2 : 32;
    while (new_cap < (size_t)need) new_cap <<= 1;

    uint8_t *new_used = (uint8_t *)realloc(used, new_cap);
    if (!new_used) return;                    /* allocation failure – abort */

    /* Initialise the new part to 0 */
    memset(new_used + used_cap, 0, new_cap - used_cap);

    used = new_used;
    used_cap = new_cap;
}

/* --- take the next smallest unused positive integer ------------------- */
static idx_t pop_next_unused(void)
{
    /* Make sure we are not looking beyond the current bitmap size. */
    while (1) {
        if ((size_t)next_unused >= used_cap) {
            ensure_used_capacity((idx_t)(next_unused + 1));
            if ((size_t)next_unused >= used_cap) {
                /* Still out of bounds – this should not happen in normal use. */
                break;
            }
        }
        if (!used[(size_t)next_unused]) break;
        ++next_unused;
    }
    idx_t val = next_unused;
    ensure_used_capacity((idx_t)(val + 1));
    used[(size_t)val] = 1;
    ++next_unused;
    return val;
}

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n=0 – safety only */

    /* Allocate/upgrade structures for the desired index. */
    ensure_seq_capacity(n);
    while (seq_len < n) {
        seq_t new_val;

        if (seq_len == 0) {                /* a(1) */
            new_val = 1;
        } else {
            idx_t idx = seq_len;            /* previous term's index (1‑based) */
            seq_t prev = seq_arr[seq_len - 1];
            if (prev == (seq_t)idx)        /* a(idx) == idx */
                new_val = 2u * idx + 1;
            else
                new_val = (seq_t)pop_next_unused();
        }

        /* Mark the value as used (again – harmless). */
        ensure_used_capacity((idx_t)(new_val + 1));
        used[(size_t)new_val] = 1;

        seq_arr[seq_len++] = new_val;
    }
    return seq_arr[n - 1];
}
