/*
 * A194461  Numbers m such that Sum_{k=1..m} (<c + k*r> - <k*r>) < 0, where
 *          r=(1+sqrt(5))/2 and c=(-1+sqrt(5))/4, and < > denotes fractional
 *          part.
 * https://oeis.org/A194461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A194461
   a(n) is the n-th integer m (m≥1) for which
   S(m) = Σ_{k=1..m} ( {c + k*r} – {k*r} ) is negative.
   Here r = (1+√5)/2, c = (√5-1)/4,  {x} denotes the fractional part.
   The criterion is equivalent to
           4·P(m) < m·(5 – √5)
   where P(m) = #{ k ≤ m : {k*r} < 1 – c }.
   This implementation builds the sequence on demand, keeping a static
   cache of already known terms.  The inequality is tested with high‐
   precision fixed‑point arithmetic (64 bits of fractional resolution),
   computed from a few long‑double constants.  The function works for
   any integer type seq_t and index type idx_t supplied by the build.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* External integer types supplied by the build */
static seq_t *seq_arr   = NULL;  /* cache of terms */
static size_t  seq_cap = 0;      /* capacity of seq_arr */
static size_t  seq_len = 0;      /* number of stored terms */

static unsigned __int128 last_m = 0; /* last processed m */
static unsigned __int128 P      = 0; /* count of favorable k */

/* Fixed‑point constants (scaled by SCALE = 2^62) */
#define SCALE ((unsigned __int128)1 << 62)

static unsigned __int128 R;           /* φ * SCALE, rounded        */
static unsigned __int128 c_scaled;    /* c * SCALE, rounded        */
static unsigned __int128 T;           /* 1 – c, scaled             */
static unsigned __int128 sqrt5_scaled;/* √5 * SCALE, rounded       */
static unsigned __int128 delta;       /* (φ – 1) * SCALE, rounded */

static void init_constants(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* Compute constants with long double (≥ 80‑bit precision). */
    const long double sqrt5L = 2.236067977499789696409173668731276235440618359611525L;
    const long double phiL   = (1.0L + sqrt5L) / 2.0L;
    const long double cL     = (-1.0L + sqrt5L) / 4.0L;

    R            = (unsigned __int128)(phiL   * SCALE + 0.5L);
    c_scaled     = (unsigned __int128)(cL     * SCALE + 0.5L);
    T            = SCALE - c_scaled;                            /* 1 – c   */
    sqrt5_scaled = (unsigned __int128)(sqrt5L * SCALE + 0.5L);
    delta        = R - SCALE;                                   /* φ – 1   */
}

/* Ensure seq_arr can hold at least 'needed' elements. */
static void grow_seq(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Return the n‑th term of A194461 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 – caller should not request 0 */

    init_constants();

    while (seq_len < (size_t)n)
    {
        /* Advance to next m */
        ++last_m;

        /* Update the fractional part of (last_m)·r:
           frac = {last_m·r}  (scaled to SCALE) */
        static unsigned __int128 frac = 0;
        frac = (frac + delta) & (SCALE - 1);

        /* Count if frac < 1 – c (i.e. frac < T). */
        if (frac < T) ++P;

        /* Test inequality 4P < last_m·(5 – √5)   (fixed‑point). */
        unsigned __int128 left  = 4 * P * SCALE;
        unsigned __int128 right = last_m * ((unsigned __int128)5 * SCALE - sqrt5_scaled);

        if (left < right)
        {
            /* last_m is a term of the sequence. */
            grow_seq(seq_len + 1);
            seq_arr[seq_len++] = (seq_t)last_m;
        }
    }

    return seq_arr[n - 1];
}
