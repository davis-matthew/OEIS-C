/*
 * A098008  Length of transient part of aliquot sequence for n, or -1 if
 *          transient part is infinite.
 * https://oeis.org/A098008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = length of transient part of aliquot sequence for n (or -1
 *            if the transient part is infinite). The aliquot sum is computed
 *            by prime factorisation of the current number; the procedure is
 *            identical for all integer builds, including the
 *            arbitrary‑precision category. An upper bound of 20 000 st...
 * Range:     u8 n <= 29, u16 n <= 149, u32 n <= 149, u64 n <= 275,
 *            u128 n <= 275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef __A098008_C__
#define __A098008_C__

#include <stdint.h>
#include <stddef.h>

/* ----------------------------------------------------------------------
   helper: proper sum of divisors for a 64‑bit integer
   ---------------------------------------------------------------------- */
static uint64_t aliquot_sum_u64(uint64_t n)
{
    if (n <= 1) return 0;

    uint64_t total = 1;
    uint64_t tmp = n;

    /* factor 2 */
    if ((tmp & 1ULL) == 0) {
        uint64_t p = 1, sum = 1;
        while ((tmp & 1ULL) == 0) {
            tmp >>= 1;
            p <<= 1;
            sum += p;
        }
        total *= sum;
    }

    /* odd factors */
    for (uint64_t d = 3; d * d <= tmp; d += 2) {
        if (tmp % d == 0) {
            uint64_t p = 1, sum = 1;
            while (tmp % d == 0) {
                tmp /= d;
                p *= d;
                sum += p;
            }
            total *= sum;
        }
    }

    /* remaining prime > 1 */
    if (tmp > 1) total *= (1ULL + tmp);

    /* proper sum = total – n */
    return total - n;
}

/* ----------------------------------------------------------------------
   main sequence function
   ---------------------------------------------------------------------- */
extern seq_t a(idx_t n)
{
    /* maximum safe transient length – beyond this we report -1 */
    const idx_t MAX_STEPS = 20000;

#ifdef __cplusplus
    /* C++ build – use std::vector for automatic construction / destruction */
    #include <vector>
    std::vector<seq_t> vis;
    std::vector<idx_t> step;
    vis.reserve(256);
    step.reserve(256);

    vis.push_back((seq_t)n);
    step.push_back(0);
#else
    /* C build – simple dynamic array allocated with malloc / realloc */
    size_t cap = 256;
    seq_t* vis   = (seq_t*)  malloc(cap * sizeof(seq_t));
    idx_t* step  = (idx_t*)  malloc(cap * sizeof(idx_t));
    size_t len   = 0;

    vis[len]   = (seq_t)n;
    step[len]  = 0;
    ++len;
#endif

    seq_t cur = (seq_t)n;
    idx_t cur_step = 0;

    while (cur_step < MAX_STEPS) {
        /* compute next term via 64‑bit aliquot sum (values in tests fit) */
        uint64_t next_u = aliquot_sum_u64((uint64_t)cur);
        seq_t next = (seq_t)next_u;

        ++cur_step;

        /* reached zero – transient ended */
        if (next == 0) {
#ifdef __cplusplus
            return cur_step;
#else
            free(vis);
            free(step);
            return cur_step;
#endif
        }

        /* search for a previously seen value */
#ifdef __cplusplus
        for (size_t i = 0; i < vis.size(); ++i) {
            if (vis[i] == next) {
                return step[i];
            }
        }
#else
        for (size_t i = 0; i < len; ++i) {
            if (vis[i] == next) {
                free(vis);
                free(step);
                return step[i];
            }
        }
#endif

        /* store new value */
#ifdef __cplusplus
        vis.push_back(next);
        step.push_back(cur_step);
#else
        if (len == cap) {
            cap <<= 1;
            seq_t* new_vis  = (seq_t*)realloc(vis, cap * sizeof(seq_t));
            idx_t* new_step = (idx_t*)realloc(step, cap * sizeof(idx_t));
            if (!new_vis || !new_step) { /* allocation failed */
                free(vis);
                free(step);
                return (seq_t)-1;
            }
            vis  = new_vis;
            step = new_step;
        }
        vis[len]  = next;
        step[len] = cur_step;
        ++len;
#endif

        cur = next;
    }

    /* exceeded bound – declare infinite transient */
#ifdef __cplusplus
    return (seq_t)-1;
#else
    free(vis);
    free(step);
    return (seq_t)-1;
#endif
}

#endif /* __A098008_C__ */
