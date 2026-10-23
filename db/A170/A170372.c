/*
 * A170372  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(0) = 1, a(n) +
 *            25·a(n-1)+⋯+25·a(n-42)+(-325)·a(n-43) = 0 for n ≥ 44 and a(43) =
 *            1 + 25·(a(1)+…+a(42)) − 325·a(0)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the rational generating function given in the OEIS entry. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* ---- initial 44 terms from the generating function ---- */
    static seq_t init[44];
    static bool ready = false;

    if (!ready) {
        init[0] = (seq_t)1;               /* p0 = 1          */
        for (size_t k = 1; k < 43; ++k) { /* a(k) for k = 1…42 */
            seq_t s = (seq_t)0;
            for (size_t i = 0; i < k; ++i)        /* sum of all previous terms */
                s += init[i];
            init[k] = (seq_t)2 + (seq_t)25 * s;   /* p_k = 2 for k ≥ 1 */
        }
        /* a(43) from the equation with p_43 = 1 */
        seq_t s = (seq_t)0;
        for (size_t i = 1; i <= 42; ++i)          /* a(42)…a(1) */
            s += init[43 - i];
        init[43] = (seq_t)1 + (seq_t)25 * s - (seq_t)325 * init[0];
        ready = true;
    }

    /* ---- fast retrieval for small n ---- */
    if (n < 44)
        return init[(size_t)n];

    /* ---- recurrence for n ≥ 44 ---- */
    static seq_t buf[44];
    for (size_t i = 0; i < 44; ++i)          /* initialise circular buffer */
        buf[i] = init[i];

    size_t N = (size_t)n;
    for (size_t idx = 44; idx <= N; ++idx) {
        seq_t sum42 = (seq_t)0;
        for (size_t k = 1; k <= 42; ++k) {
            size_t j = (idx - 1 + 44 - k) % 44;  /* a(idx-k) */
            sum42 += buf[j];
        }
        size_t j43 = (idx - 43 + 44) % 44;       /* a(idx-43) */
        seq_t val = (seq_t)25 * sum42 - (seq_t)325 * buf[j43];
        buf[idx % 44] = val;
    }
    return buf[N % 44];
}
