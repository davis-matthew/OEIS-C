/*
 * A100298  Similar to Cald's sequence (A006509), but move by the n-th
 *          composite instead of by the n-th prime.
 * https://oeis.org/A100298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1)=1.
 * For n>1 let c_{n-1} be the (n-1)-th composite number.
 * If a(n-1)−c_{n-1} is positive and has not appeared earlier,
 *    a(n)=a(n-1)−c_{n-1};
 * else if a(n-1)+c_{n-1} has not appeared earlier, 
 *    a(n)=a(n-1)+c_{n-1};
 * else a(n)=0.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime.h"

static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

static seq_t *comps = NULL;
static size_t comps_len = 0;
static size_t comps_cap = 0;
static uint64_t next_comp_candidate = 4; /* first composite */

/* linear search; seq holds all values up to upto-1 */
static bool value_seen(seq_t val, size_t upto)
{
    for (size_t i = 0; i < upto; ++i)
        if (seq[i] == val) return true;
    return false;
}

static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 1;
    while (new_cap < needed) new_cap <<= 1;
    seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

static void ensure_comps_capacity(size_t needed)
{
    if (needed <= comps_cap) return;
    size_t new_cap = comps_cap ? comps_cap : 1;
    while (new_cap < needed) new_cap <<= 1;
    comps = (seq_t*)realloc(comps, new_cap * sizeof(seq_t));
    comps_cap = new_cap;
}

static void generate_composites_up_to(size_t count)
{
    while (comps_len < count) {
        /* find next composite */
        while (is_prime(next_comp_candidate)) ++next_comp_candidate;
        ensure_comps_capacity(comps_len + 1);
        comps[comps_len++] = (seq_t)next_comp_candidate;
        ++next_comp_candidate;
    }
}

static void compute_up_to(idx_t n)
{
    if (n == 0) return;
    if ((size_t)n <= seq_len) return;

    ensure_seq_capacity((size_t)n);

    if (seq_len == 0) {
        seq[0] = (seq_t)1;   /* a(1) */
        seq_len = 1;
    }

    for (idx_t k = seq_len; k < n; ++k) {
        /* ensure we have at least k composites (k-1 needed for a(k+1)) */
        generate_composites_up_to((size_t)k);

        seq_t step = comps[(size_t)(k - 1)];   /* c_{k-1} */
        seq_t prev = seq[(size_t)(k - 1)];     /* a(k) */

        seq_t cand1 = 0;
        bool cand1_ok = false;
        if (prev > step) {
            cand1 = prev - step;
            if (!value_seen(cand1, (size_t)k)) cand1_ok = true;
        }

        if (cand1_ok) {
            seq[(size_t)k] = cand1;
        } else {
            seq_t cand2 = prev + step;
            if (!value_seen(cand2, (size_t)k))
                seq[(size_t)k] = cand2;
            else
                seq[(size_t)k] = (seq_t)0;
        }
    }
    seq_len = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    compute_up_to(n);
    return seq[(size_t)(n - 1)];
}
