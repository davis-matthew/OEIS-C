/*
 * A063949  Every number is the sum of 4 squares; these are the numbers n for
 *          which the first square can be taken to be any positive square < n.
 * https://oeis.org/A063949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) is the nth number (offset 1) that can be expressed as a sum
 *            of four squares with the first square being any positive square
 *            < n. For every integer k satisfying 1 ≤ k < sqrt(n), the
 *            remaining integer n – k² must NOT be of the form 4^a·(8b+7), by
 *            Legendre's three‑square theorem. The function b...
 * Range:     u8 n <= 111, u16 n <= 1109, u32 n <= 1109, u64 n <= 1109,
 *            u128 n <= 1109, big any n
 * Helpers:   can_be_three_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/can_be_three_squares.h"

/* ------------------------------------------------------------------ */
/*  Integer square root (floor) for 64‑bit unsigned integers           */
/* ------------------------------------------------------------------ */
static idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == n)
            return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Dynamic array storing the computed sequence terms.                */
/* ------------------------------------------------------------------ */
static seq_t *seq      = NULL;   /* array of sequence values          */
static size_t seq_cap  = 0;       /* current capacity                   */
static size_t seq_len  = 0;       /* number of stored terms             */
static idx_t   last_checked = 0; /* next integer candidate to test    */

/* Ensure the sequence buffer can hold at least 'needed' elements.    */
static void ensure_seq_capacity(size_t needed)
{
    if (seq_cap >= needed)
        return;
    size_t newcap = seq_cap ? seq_cap << 1 : 64;
    while (newcap < needed)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
    if (!tmp) abort(); /* allocation failure */
    seq = tmp;
    seq_cap = newcap;
}

/* Generate terms until the array contains at least 'target' elements.   */
static void generate_until(size_t target)
{
    while (seq_len < target)
    {
        idx_t candidate = last_checked++;
        bool ok = true;
        if (candidate > 0)
        {
            idx_t max_k = isqrt_idx(candidate - 1);
            for (idx_t k = 1; k <= max_k; ++k)
            {
                idx_t m = candidate - k * k;
                if (!can_be_three_squares(m))
                {
                    ok = false;
                    break;
                }
            }
        }
        if (ok)
        {
            ensure_seq_capacity(seq_len + 1);
            seq[seq_len++] = (seq_t)candidate;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Public function required by the exercise.                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)        /* undefined offset, return 0 as safe default */
        return (seq_t)0;

    /* One‑time initialization of the cache */
    if (!seq)
    {
        ensure_seq_capacity(512);
        seq_len = 0;
        last_checked = 0;
    }

    /* Grow the cache to satisfy the requested index */
    if (seq_len < n)
        generate_until(n);

    return seq[n - 1];  /* offset 1: a(1) is first stored term */
}
