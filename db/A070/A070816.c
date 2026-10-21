/*
 * A070816  Solutions to phi(gpf(x)) - gpf(phi(x)) = 65534 = c are special
 *          multiples of 65537, x=65537*k, where the largest prime factors of
 *          factor k were observed in {2, 3, 5, 17, 257}.
 * https://oeis.org/A070816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A070816 – Solutions to φ(gpf(x)) – gpf(φ(x)) = 65534.
    x = 65537 * k, where k is of the form 2^a * Π p_i
    (p_i ∈ {3,5,17,257}, each p_i used at most once).
    The sequence is
    a(n) = 65537 * k_n
    where k_n runs over all such numbers in increasing order.  */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *smooth = NULL;          /* array of k values */
static size_t smooth_capacity = 0;    /* allocated capacity */
static size_t smooth_len = 0;         /* number of elements currently stored */

static seq_t cur[16];                 /* current value in each 2^a‑stream */
static seq_t subset[16];              /* base products of {3,5,17,257} */
static bool initialized = false;

static inline void ensure_capacity(size_t needed)
{
    if (smooth_capacity < needed)
    {
        size_t new_cap = smooth_capacity ? smooth_capacity << 1 : 16;
        if (new_cap < needed) new_cap = needed;
        smooth = (seq_t *)realloc(smooth, new_cap * sizeof(seq_t));
        smooth_capacity = new_cap;
    }
}

static inline void init_once(void)
{
    if (initialized) return;
    initialized = true;

    /* precompute all subset products of {3,5,17,257} (0 to 15) */
    for (int mask = 0; mask < 16; ++mask)
    {
        seq_t val = 1;
        if (mask & 1) val *= 3;
        if (mask & 2) val *= 5;
        if (mask & 4) val *= 17;
        if (mask & 8) val *= 257;
        subset[mask] = val;
        cur[mask]   = val;          /* initial 2^0 stream value */
    }

    ensure_capacity(16);
    smooth_len = 0;
}

/* generate next k values until we have n of them */
static inline void generate_k(const idx_t n)
{
    while (smooth_len < n)
    {
        /* find smallest current stream value */
        size_t minidx = 0;
        seq_t minval  = cur[0];
        for (size_t i = 1; i < 16; ++i)
        {
            if (cur[i] < minval)
            {
                minval = cur[i];
                minidx = i;
            }
        }

        ensure_capacity(smooth_len + 1);
        smooth[smooth_len++] = minval;

        /* advance the chosen stream by one power of two */
        cur[minidx] = cur[minidx] * 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of bounds, not used by OEIS */
    init_once();
    generate_k((idx_t)n);
    return (seq_t)65537 * smooth[n - 1];
}
