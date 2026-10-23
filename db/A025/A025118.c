/*
 * A025118  a(n) = s(1)s(n) + s(2)s(n-1) + ... + s(k)s(n-k+1), where k = [ n/2
 *          ], s = A000201 (lower Wythoff sequence).
 * https://oeis.org/A025118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} ⌊i·φ⌋ · ⌊(n-i+2)·φ⌋, φ = (1+√5)/2 *
 *            (This shift makes a(1)=3, a(2)=4, … as listed in the OEIS
 *            entry.)
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* 1‑based cache of the lower Wythoff sequence s(n) = ⌊n·φ⌋. */
static seq_t *s_cache = NULL;  /* s_cache[0] is unused */
static size_t s_len   = 0;     /* largest index stored */

/* Extend the cache to contain values up to n (inclusive). */
static void ensure_s_cache(idx_t n)
{
    /* The cache is already sufficient? */
    if (s_len >= (size_t)n)
        return;

    size_t new_len = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(s_cache, (new_len + 1) * sizeof(seq_t));
    if (!tmp)  /* allocation failure – leave cache unchanged */
        return;
    s_cache = tmp;

    /* Constant φ as a double.  This constant is accurate to far more
     * than the precision needed for n up to 10 000. */
    const double phi = 1.6180339887498948482045868;

    /* Compute ⌊i·φ⌋ for all new indices.  We use a temporary machine
     * integer (uint64_t) then cast to seq_t; this works for both
     * fixed‑size and arbitrary‑precision integer builds. */
    for (size_t i = s_len + 1; i <= new_len; ++i) {
        uint64_t tmp_val = (uint64_t)((double)i * phi + 1e-12);
        s_cache[i] = (seq_t)tmp_val;
    }
    s_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset 1 ⇒ a(1) is first term */

    /* Because of the shift needed to match the OEIS sample data,
     * compute the sum for n+1. */
    idx_t target = n + 1;
    ensure_s_cache(target);

    size_t k = (size_t)(target / 2);
    seq_t sum = (seq_t)0;
    for (size_t i = 1; i <= k; ++i) {
        sum += s_cache[i] * s_cache[(size_t)(target - i + 1)];
    }
    return sum;
}
