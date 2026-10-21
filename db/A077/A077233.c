/*
 * A077233  a(n) is smallest natural number a satisfying Pell equation b^2-
 *          d(n)*a^2= +1 or = -1, with d(n)=A000037(n) (nonsquare).
 *          Corresponding smallest b(n)=A077232(n).
 * https://oeis.org/A077233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 196, u64 n <= 605, u128 n <= 1966,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A077233
 *  a(n) = smallest positive integer a such that
 *         b^2 – d*a^2 = ±1, where
 *         d is the n‑th non‑square integer (A000037)
 *  (b is the corresponding smallest positive integer, A077232)
 *
 *  For even period of sqrt(d) we search for the first +1 solution;
 *  for odd period the -1 solution gives a = 1.
 *
 *  The function builds d on demand from n and then uses a
 *  continued‑fraction expansion of sqrt(d) to find the minimal
 *  solution of the Pell equation.  The implementation avoids
 *  any floating‑point operations and is valid for all supported
 *  integer sizes (uint8_t … __int128).  All arithmetic is performed
 *  in seq_t; intermediate 64‑bit values are cast explicitly when
 *  necessary.
 */

#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Utility: floor square root for uint64_t (works for all seq_t <= 128) */
/* ------------------------------------------------------------------ */
static inline uint64_t _isqrt_u64(uint64_t n)
{
    /* Newton’s method – safe for all 64‑bit values */
    uint64_t x = n, y = (x + 1) / 2;
    while (y < x)
    {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

/* ------------------------------------------------------------------ */
/* Find the n‑th positive non‑square integer.                           */
/* ------------------------------------------------------------------ */
static idx_t nth_non_square(idx_t n)          /* n >= 1 */
{
    idx_t low  = n;
    idx_t high = n * 2;

    /* Grow high until it surely contains the n-th non‑square */
    while (high - _isqrt_u64((uint64_t)high) < n)
        high <<= 1;

    /* Binary search for smallest x with count_of_non_squares(x) >= n */
    while (low < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (mid - _isqrt_u64((uint64_t)mid) < n)
            low = mid + 1;
        else
            high = mid;
    }

    /* If low is a perfect square, skip it */
    uint64_t r = _isqrt_u64((uint64_t)low);
    if (r * r == (uint64_t)low)
        ++low;

    return low;
}

/* ------------------------------------------------------------------ */
/* Smallest a for Pell equation x^2 – d*a^2 = ±1                         */
/* ------------------------------------------------------------------ */
static seq_t min_a_of_pell(idx_t d)            /* d is nonsquare */
{
    /* Special case: check the trivial convergent (a0,1) */
    idx_t a0  = _isqrt_u64((uint64_t)d);       /* floor(sqrt(d)) */
    seq_t p   = (seq_t)a0;                     /* p0  */
    seq_t q   = (seq_t)1;                      /* q0  */
    seq_t diff = p*p - d*q*q;
    if (diff == 0)        /* should not happen for nonsquare d */
        return 0;
    if (diff == 1 || diff == -1)
        return q;          /* a = 1 for odd period (or even with +1) */

    /* Initialize recurrence variables */
    seq_t p_pp = (seq_t)1;                    /* p_{-1} */
    seq_t q_pp = (seq_t)0;                    /* q_{-1} */
    seq_t p_prevprev = p_pp;
    seq_t p_prev = p;                         /* p0  */
    seq_t q_prevprev = q_pp;
    seq_t q_prev = q;                         /* q0  */

    /* CF variables */
    idx_t m  = 0;
    idx_t dcoeff = 1;
    idx_t a = a0;

    for (;;) {
        /* Compute next CF coefficient */
        idx_t m_next = dcoeff * a - m;
        idx_t dcoeff_next = (d - m_next * m_next) / dcoeff;
        idx_t a_next = (a0 + m_next) / dcoeff_next;

        /* Compute next convergent */
        seq_t p_next = (seq_t)a_next * p_prev + p_prevprev;
        seq_t q_next = (seq_t)a_next * q_prev + q_prevprev;

        /* Check Pell equation for this convergent */
        seq_t diff = p_next*p_next - d*q_next*q_next;
        if (diff == 1 || diff == -1)
            return q_next;      /* minimal a found */

        /* Prepare for next iteration */
        p_prevprev = p_prev;
        p_prev = p_next;
        q_prevprev = q_prev;
        q_prev = q_next;
        m = m_next;
        dcoeff = dcoeff_next;
        a = a_next;
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence generator                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)   /* the sequence is 1‑indexed; behaviour here is undefined */
        return 0;
    idx_t d = nth_non_square(n);   /* d = A000037(n) */
    return min_a_of_pell(d);       /* minimal a for Pell eq. */
}
