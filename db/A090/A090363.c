/*
 * A090363  Shifts 1 place left under the BINOMIAL transform of the
 *          self-convolution of this sequence.
 * https://oeis.org/A090363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0}^{n-1} ( Sum_{i=0}^{k} b_i * b_{k-i} ) where b_0
 *            = 1 and b_k = Sum_{t=1}^{k} a_t * C(k-1, t-1). The sequence
 *            satisfies the G.f. A(x) = 1 + A(x/(1-x))^2 * x/(1-x)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation: use std::vector to hold the values and
       let the compiler call constructors for arbitrary‑precision
       types. */
    static std::vector<seq_t> Ao, Bb, Ff, CF;
    static idx_t computed = 0;

    if (Ao.size() <= n) {
        size_t new_cap = n + 1;
        Ao.resize(new_cap);
        Bb.resize(new_cap);
        Ff.resize(new_cap);
        CF.resize(new_cap);
    }

    if (computed == 0) {
        Ao[0] = 1;
        Bb[0] = 1;
        Ff[0] = Ao[0] * Ao[0];
        CF[0] = Ff[0];
        computed = 0;
    }

    for (idx_t k = computed + 1; k <= n; ++k) {
        /* a(k) */
        Ao[k] = CF[k - 1];

        /* b(k) = Σ_{t=1}^{k} a(t) * C(k−1, t−1) */
        seq_t binom = 1;
        seq_t sum_b = 0;
        for (idx_t t = 1; t <= k; ++t) {
            sum_b += Ao[t] * binom;
            if (t < k) binom = binom * (k - t) / t;
        }
        Bb[k] = sum_b;

        /* f(k) = Σ_{i=0}^{k} b(i) * b(k−i)  (convolution) */
        seq_t sum_f = 0;
        for (idx_t i = 0; i <= k; ++i) sum_f += Bb[i] * Bb[k - i];
        Ff[k] = sum_f;

        /* cumulative sum of f */
        CF[k] = CF[k - 1] + Ff[k];
    }

    computed = n;
    return Ao[n];
#else
    /* C implementation: use malloc/realloc to manage the arrays.
       All operations are performed on POD types, which is valid for
       both integer and arbitrary‑precision types that are POD. */
    static seq_t *Ao = NULL, *Bb = NULL, *Ff = NULL, *CF = NULL;
    static idx_t cap = 0, computed = 0;

    /* Ensure the arrays have space for indices 0..n */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        Ao = (seq_t*)realloc(Ao, new_cap * sizeof(seq_t));
        Bb = (seq_t*)realloc(Bb, new_cap * sizeof(seq_t));
        Ff = (seq_t*)realloc(Ff, new_cap * sizeof(seq_t));
        CF = (seq_t*)realloc(CF, new_cap * sizeof(seq_t));
        for (idx_t i = cap; i < new_cap; ++i) {
            Ao[i] = 0;
            Bb[i] = 0;
            Ff[i] = 0;
            CF[i] = 0;
        }
        cap = new_cap;
    }

    if (computed == 0) {
        Ao[0] = 1;
        Bb[0] = 1;
        Ff[0] = Ao[0] * Ao[0];
        CF[0] = Ff[0];
        computed = 0;
    }

    for (idx_t k = computed + 1; k <= n; ++k) {
        /* a(k) */
        Ao[k] = CF[k - 1];

        /* b(k) = Σ_{t=1}^{k} a(t) * C(k−1, t−1) */
        seq_t binom = 1;
        seq_t sum_b = 0;
        for (idx_t t = 1; t <= k; ++t) {
            sum_b += Ao[t] * binom;
            if (t < k) binom = binom * (k - t) / t;
        }
        Bb[k] = sum_b;

        /* f(k) = Σ_{i=0}^{k} b(i) * b(k−i) */
        seq_t sum_f = 0;
        for (idx_t i = 0; i <= k; ++i) sum_f += Bb[i] * Bb[k - i];
        Ff[k] = sum_f;

        /* cumulative sum of f */
        CF[k] = CF[k - 1] + Ff[k];
    }

    computed = n;
    return Ao[n];
#endif
}
