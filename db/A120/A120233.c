/*
 * A120233  Split-floor-multiplier sequence (SFMS) using multipliers 3/4 and
 *          4/3. (SFMS is defined at A120229.).
 * https://oeis.org/A120233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(3·n/4) if that value is positive and has not *
 *            appeared before, otherwise a(n) = floor(4·n/3). This
 *            implementation * remembers all previously generated values and
 *            uses a bit‑array to * test whether a candidate has already been
 *            used. The code works * for any seq_t/idx_t types provided...
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ------------  state  ------------ */

/* Array of the sequence values, 1‑based; elements are filled
 * lazily.  seq_used[i] == 1 means seq_cache[i] has been computed. */
static seq_t   *seq_cache  = NULL;
static unsigned char *seq_used = NULL;
static size_t      seq_cap   = 0;     /* allocated length of seq_* arrays */

/* Bit‑array that records which integer values have appeared.
 * Index of a value is used directly (value may be cast to size_t;
 * this works well enough for the ranges tested). */
static uint8_t *used_bits  = NULL;   /* bytes holding the bits */
static size_t   used_bits_len = 0;   /* number of bits currently stored */

/* ------------  helpers  ------------ */

static inline bool is_used(seq_t val)
{
    size_t idx = (size_t)val;
    if (idx >= used_bits_len)
        return false;
    return (used_bits[idx >> 3] >> (idx & 7)) & 0x01u;
}

static void mark_used(seq_t val)
{
    size_t idx = (size_t)val;
    size_t needed_bits = idx + 1;
    if (used_bits_len < needed_bits) {
        size_t new_bits = used_bits_len ? used_bits_len * 2 : 64;
        if (new_bits < needed_bits) new_bits = needed_bits;
        size_t new_bytes = (new_bits + 7) >> 3;
        size_t old_bytes = (used_bits_len + 7) >> 3;
        uint8_t *tmp = (uint8_t *)realloc(used_bits, new_bytes);
        if (!tmp) return;              /* allocation failed; ignore */
        if (new_bytes > old_bytes)
            memset(tmp + old_bytes, 0, new_bytes - old_bytes);
        used_bits      = tmp;
        used_bits_len  = new_bits;
    }
    size_t byte = idx >> 3;
    used_bits[byte] |= (1u << (idx & 7));
}

/* Resize seq_cache and seq_used to at least new_cap elements.  */
static bool resize_seq_cache(size_t new_cap)
{
    if (new_cap <= seq_cap) return true;

    seq_t   *tmp_cache  = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp_cache) return false;
    seq_cache = tmp_cache;

    unsigned char *tmp_used = (unsigned char *)realloc(seq_used, new_cap);
    if (!tmp_used) return false;
    seq_used = tmp_used;
    memset(tmp_used + seq_cap, 0, new_cap - seq_cap);   /* new entries unused */

    seq_cap = new_cap;
    return true;
}

/* ------------  public function  ------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n ≥ 1 */

    size_t target = (size_t)n;
    if (!resize_seq_cache(target + 1)) return 0;   /* allocation failed */

    /* Generate missing terms up to target. */
    for (size_t i = 1; i <= target; ++i) {
        if (seq_used[i]) continue;             /* already computed */

        /* Candidate 1 : floor(3*i/4) */
        seq_t cand1 = ((seq_t)i * 3u) / 4u;
        seq_t val;
        if (cand1 > 0 && !is_used(cand1)) {
            val = cand1;
        } else {
            seq_t cand2 = ((seq_t)i * 4u) / 3u;
            val = cand2;
        }

        mark_used(val);
        seq_cache[i] = val;
        seq_used[i] = 1;
    }

    return seq_cache[target];
}
