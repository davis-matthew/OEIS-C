/*
 * A178913  a(n) is the number of integer bases r, where r is less than or
 *          equal to n, in which n cannot be expressed in the form of
 *          k+Reverse(k) for any k.
 * https://oeis.org/A178913
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A178913
   Formula: a(n) = (n-1) – (# of bases r, 2 ≤ r ≤ n, for which
   ∃k ≥ 0: k + Reverse_r(k) = n) */

#include <stdint.h>
#include <stdlib.h>

/* internal state for memoisation */
static struct {
    idx_t        maxN;       /* highest n that has been computed */
    seq_t       *a_cache;    /* a(n) for 0 ≤ n ≤ maxN */
    size_t     *cnt_set;    /* cnt_set[n] = # of bases r ≤ n that can produce n */
    uint32_t   *last_seen;  /* auxiliary array: last r that produced each n */
} state = {0, NULL, NULL, NULL};

/* rebuild tables up to Nmax (inclusive) */
static void rebuild(idx_t Nmax)
{
    if (Nmax <= state.maxN)
        return;

    /* free previous storage if any */
    free(state.a_cache);
    free(state.cnt_set);
    free(state.last_seen);

    /* allocate new storage */
    state.a_cache  = (seq_t*)calloc((size_t)Nmax + 1, sizeof(seq_t));
    state.cnt_set  = (size_t*)calloc((size_t)Nmax + 1, sizeof(size_t));
    state.last_seen = (uint32_t*)calloc((size_t)Nmax + 1, sizeof(uint32_t));

    /* compute reachable sums for each base r */
    for (idx_t r = 2; r <= Nmax; ++r) {
        uint64_t base = (uint64_t)r;          /* base is small, 64‑bit is safe */

        /* consider all k from 0 .. Nmax */
        for (idx_t k = 0; k <= Nmax; ++k) {
            uint64_t v   = (uint64_t)k;
            uint64_t rev = 0;

            /* reverse k in base r */
            while (v) {
                rev = rev * base + (v % base);
                v   /= base;
            }

            uint64_t s = (uint64_t)k + rev;
            if (s > (uint64_t)Nmax)
                continue;
            /* we only count base r if r ≤ s (i.e., r ≤ n) */
            if (r <= (idx_t)s) {
                /* mark s as produced by this r only once */
                if (state.last_seen[(size_t)s] != (uint32_t)r) {
                    state.last_seen[(size_t)s] = (uint32_t)r;
                    state.cnt_set[(size_t)s]++;   /* increment base counter */
                }
            }
        }
    }

    /* fill a_cache from the counted sets */
    for (idx_t n = 2; n <= Nmax; ++n) {
        seq_t base_cnt = (seq_t)(n - 1);                 /* bases 2..n */
        seq_t used_cnt = (seq_t)state.cnt_set[(size_t)n];/* bases that CAN produce n */
        state.a_cache[(size_t)n] = base_cnt - used_cnt;  /* bases that CANNOT */
    }

    state.maxN = Nmax;
}

/* sequence generator: a(n) for n ≥ 2 */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* undefined below offset */
    if (n > state.maxN)
        rebuild(n);
    return state.a_cache[(size_t)n];
}
