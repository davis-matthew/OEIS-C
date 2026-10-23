/*
 * A107779  a(1)=8, a(n) = smallest integer not previously used which contains
 *          a digit from a(n-1).
 * https://oeis.org/A107779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 8; a(n) = smallest unused non‑negative integer that
 *            contains at least one digit from a(n‑1), for n > 1. (Recurrence)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- auxiliary: digit mask (handles 0 correctly) ---------- */
static inline uint16_t digit_mask(seq_t x)
{
    if (x == (seq_t)0)
        return 1u << 0;         /* digit 0 */
    uint16_t mask = 0;
    while (x != (seq_t)0) {
        int d = (int)(x % (seq_t)10);   /* digit 0‑9 */
        mask |= 1u << d;
        x /= (seq_t)10;
    }
    return mask;
}

/* ---------- dynamic array of sequence values ---------- */
static idx_t max_n   = 0;          /* number of terms already computed      */
static seq_t *seq_arr = NULL;      /* storage for the terms                 */
static size_t seq_cap = 0;         /* capacity of seq_arr                    */

static void ensure_seq_cap(void)
{
    if (max_n >= seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 16 : seq_cap << 1;
        seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* out of memory */
        seq_arr = tmp;
        seq_cap = new_cap;
    }
}

static void push_seq(seq_t val)
{
    ensure_seq_cap();
    seq_arr[max_n++] = val;
}

/* ---------- bit‑set of used integers ---------- */
static unsigned char *used_bits = NULL;  /* bit i = 1 ⇔ value i has appeared */
static size_t used_cap = 0;              /* size of bit‑set in bits            */

static void ensure_used_cap(idx_t idx)
{
    if ((size_t)idx >= used_cap) {
        size_t new_cap = (used_cap == 0) ? 64 : used_cap << 1;
        while ((size_t)idx >= new_cap) new_cap <<= 1;
        unsigned char *tmp = (unsigned char *)realloc(used_bits,
                                                      new_cap >> 3);
        if (!tmp) abort();                /* out of memory */
        /* clear the newly allocated bytes */
        for (size_t i = used_cap >> 3; i < (new_cap >> 3); ++i)
            tmp[i] = 0;
        used_bits = tmp;
        used_cap  = new_cap;
    }
}

static inline bool is_used(idx_t idx)
{
    if ((size_t)idx >= used_cap) return false;
    return (used_bits[idx >> 3] >> (idx & 7)) & 0x01;
}

static inline void set_used(idx_t idx)
{
    ensure_used_cap(idx);
    used_bits[idx >> 3] |= (1 << (idx & 7));
}

/* ---------- bookkeeping between iterations ---------- */
static seq_t last_used_val = 0;   /* a(n‑1) for generating the next term */

/* ---------- sequence generator ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, safeguard */

    /* initialise first term if not yet done */
    if (max_n == 0) {
        push_seq((seq_t)8);
        set_used((idx_t)8);
        last_used_val = (seq_t)8;
    }

    /* compute additional terms up to n (out‑of‑order calls are handled) */
    while (max_n < n) {
        uint16_t mask_last = digit_mask(last_used_val);
        idx_t cand_idx = 0;
        for ( ; ; ++cand_idx) {
            if (is_used(cand_idx))
                continue;
            seq_t cand_val = (seq_t)cand_idx;
            if ((digit_mask(cand_val) & mask_last) != 0) {
                /* found suitable candidate */
                push_seq(cand_val);
                set_used(cand_idx);
                last_used_val = cand_val;
                break;
            }
        }
    }

    return seq_arr[n - 1];
}
