/*
 * A025108  s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2 ], s =
 *          (F(2), F(3), F(4), ...), t = A001950 (upper Wythoff sequence).
 * https://oeis.org/A025108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} F_{i+1} · ⌊(n - i + 2)·φ²⌋ where
 *            F_{k} is the k‑th Fibonacci number (F₁ = 1, F₂ = 1) and φ =
 *            (1+√5)/2
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   upper_wythoff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/upper_wythoff.h"

/* The main sequence function. */
seq_t a(idx_t n)
{
    /* The OEIS definition uses index starting at 1, but the given
       formula yields the values for n+1.  Therefore we shift the
       argument by one. */
    idx_t m = n + 1U;          /* m = n + 1  */
    idx_t k = m / 2U;          /* floor(m/2) */

    seq_t result = 0;

    /* Fibonacci iteration: a = F(1), b = F(2).  For i=1 we need F(2). */
    seq_t a = (seq_t)1;        /* F(1) */
    seq_t b = (seq_t)1;        /* F(2) */

    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t s = b;                      /* s(i) = F(i+1) = b */
        idx_t t_idx = m - i + 1U;         /* index for t(n-i+1) */
        seq_t t_val = upper_wythoff(t_idx);

        result += s * t_val;

        /* Update Fibonacci: advance to next two terms. */
        seq_t next = a + b;
        a = b;
        b = next;
    }

    return result;
}
