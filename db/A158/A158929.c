/*
 * A158929  a(n) is the smallest integer not yet in the sequence with no
 *          common base-4 digit with a(n-1).
 * https://oeis.org/A158929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A158929
   a(1) = 1
   a(n) = smallest integer not yet in the sequence whose base‑4
   representation shares no digit with a(n‑1).  The sequence is
   generated lazily; any order of calls is supported.  It works
   for the integer types seq_t and idx_t supplied by the build
   system (including arbitrary‑precision seq_t). */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- sequence storage ---------- */
static seq_t *seq      = NULL;   /* array of terms          */
static size_t seq_len  = 0;      /* number of terms stored  */
static size_t seq_cap  = 0;      /* allocated capacity      */

/* grow the sequence array to hold at least 'needed' terms */
static void seq_grow(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (seq) {
        memcpy(new_arr, seq, seq_len * sizeof(seq_t));
        free(seq);
    }
    seq = new_arr;
    seq_cap = new_cap;
}

/* ---------- used‑bitset storage ---------- */
static uint64_t *used_bits_arr = NULL; /* bit  i == 1 → number i is used */
static size_t   used_bits_size = 0;    /* total number of bits allocated   */
static size_t   used_words     = 0;    /* used_bits_arr size in words      */

/* ensure the bitset can address bit 'v' (0‑based).  */
static void ensure_used_capacity(uint64_t v)
{
    if (v < used_bits_size) return;
    size_t new_bits = used_bits_size ? used_bits_size << 1 : 64;
    while (new_bits <= v) new_bits <<= 1;
    size_t new_words = (new_bits + 63) >> 6;
    uint64_t *new_arr = (uint64_t *)calloc(new_words, sizeof(uint64_t));
    if (used_bits_arr) {
        memcpy(new_arr, used_bits_arr, used_words * sizeof(uint64_t));
        free(used_bits_arr);
    }
    used_bits_arr = new_arr;
    used_bits_size = new_bits;
    used_words = new_words;
}

/* test whether number v has been used */
static bool used_get(uint64_t v)
{
    return (used_bits_arr[v >> 6] >> (v & 63)) & 1ULL;
}

/* mark number v as used */
static void used_set(uint64_t v)
{
    if (v >= used_bits_size) ensure_used_capacity(v);
    used_bits_arr[v >> 6] |= 1ULL << (v & 63);
}

/* ---------- helper routines ---------- */

/* return bitmask of base‑4 digits present in x */
static uint8_t digit_mask(seq_t x)
{
    uint8_t m = 0;
    if (x == 0) return 1;           /* digit 0 */
    const seq_t four = (seq_t)4;
    while (x > 0) {
        unsigned d = (unsigned)(x % four);
        m |= 1 << d;
        x /= four;
    }
    return m;
}

/* test that all base‑4 digits of x belong to 'mask' */
static bool digits_allowed(seq_t x, uint8_t mask)
{
    const seq_t four = (seq_t)4;
    while (x > 0) {
        unsigned d = (unsigned)(x % four);
        if (!(mask & (1 << d))) return false;
        x /= four;
    }
    return true;
}

/* ---------- sequence generator ---------- */

/* generate the next term in the sequence */
static seq_t generate_next(void)
{
    seq_t prev = seq[seq_len - 1];
    uint8_t prev_mask = digit_mask(prev);
    uint8_t allowed_mask = (~prev_mask) & 0xF;          /* complement of digits in prev */

    static seq_t last_used[16] = {0};

    seq_t cand = last_used[allowed_mask] + 1;

    while (true) {
        uint64_t idx = (uint64_t)cand;
        if (idx >= used_bits_size) ensure_used_capacity(idx);
        if (!used_get(idx) && digits_allowed(cand, allowed_mask)) break;
        ++cand;
    }

    /* add to sequence */
    if (seq_len == seq_cap) seq_grow(seq_cap ? seq_cap << 1 : 4);
    seq[seq_len++] = cand;
    used_set((uint64_t)cand);
    last_used[allowed_mask] = cand;
    return cand;
}

/* ---------- public function required by the build system ---------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 */

    /* initialise first term lazily */
    while (seq_len < n) {
        if (seq_len == 0) {
            seq_grow(4);
            seq[0] = (seq_t)1;
            used_set(1);
            seq_len = 1;
        } else {
            generate_next();
        }
    }
    return seq[n - 1];
}
