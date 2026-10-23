/*
 * A185602  Numbers not of the form floor(n^(3/2))-floor(n^(1/2))-n;
 *          complement of A185601.
 * https://oeis.org/A185602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 146, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A185602.
    a(n) = the n‑th positive integer NOT of the form
            floor(m^(3/2)) – floor(m^(1/2)) – m  (m ≥ 3),
    with offset 1.
    The function builds the sequence incrementally and memoises the
    complementary values, so that calls with arbitrary n are fast. */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Integer square root for 64‑bit values.  Uses 128‑bit arithmetic to
   avoid overflow in the product. */
static idx_t floor_sqrt(idx_t x)
{
    idx_t r = 0;
    for (int bit = 63; bit >= 0; --bit) {
        idx_t next = r | (1ULL << bit);
        if ((unsigned __int128)next * next <= x)
            r = next;
    }
    return r;
}

/* Integer square root for 128‑bit values, result fits in 64 bits. */
static idx_t isqrt_u128(unsigned __int128 x)
{
    idx_t low = 0, high = (idx_t)~0ULL;
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* Evaluate f(m) = floor(m^(3/2)) – floor(m^(1/2)) – m for m≥3.
   All arithmetic is performed in 128‑bit when necessary to avoid
   overflow.  The result is cast to seq_t only after the 128‑bit
   subtraction, guaranteeing correctness for values that fit in
   seq_t. */
static seq_t f_calc(idx_t m)
{
    idx_t s = floor_sqrt(m);                     /* floor(sqrt(m)) */
    unsigned __int128 m3 = (unsigned __int128)m * m * m;
    idx_t t = isqrt_u128(m3);                   /* floor(sqrt(m^3)) */
    unsigned __int128 res128 = (unsigned __int128)t - s - m; /* ≥0 for m≥3 */
    return (seq_t)res128;
}

/* ------------------------------------------------------------------ */
/* Public sequence function.  Static state is maintained so that
   subsequent calls are efficient regardless of order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* out of spec, safety */

    /* Static memoised state */
    static idx_t          curr_x      = 1;      /* integer being tested */
    static idx_t          next_m      = 3;      /* next m for f(m)        */
    static seq_t          next_f_val  = 0;      /* f(next_m)              */
    static bool           f_valid     = false;  /* f_val computed?        */
    static seq_t *        comps       = NULL;   /* stored complement values */
    static size_t         comps_len   = 0;      /* number stored          */
    static size_t         comps_cap   = 0;      /* allocated size         */

    /* Compute first f_val on first entry. */
    if (!f_valid) {
        next_f_val = f_calc(next_m);
        f_valid = true;
    }

    /* Extend the table until we have at least n terms. */
    while (comps_len < n) {
        /* Advance f(m) until it meets or exceeds curr_x. */
        while (next_f_val < curr_x) {
            ++next_m;
            next_f_val = f_calc(next_m);
        }

        /* If current integer is forbidden, skip it. */
        if (next_f_val == curr_x) {
            ++curr_x;
            continue;
        }

        /* Store a valid complement number. */
        if (comps_len == comps_cap) {
            comps_cap = comps_cap ? comps_cap * 2 : 64;
            comps = (seq_t *)realloc(comps, comps_cap * sizeof(seq_t));
        }
        comps[comps_len++] = (seq_t)curr_x;
        ++curr_x;
    }

    return comps[n - 1];
}
