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
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the upper Wythoff number t(i) = ⌊i·φ²⌋.
   We use the identity φ² = φ + 1, so t(i) = i + ⌊i·φ⌋.
   ⌊i·φ⌋ is evaluated by integer arithmetic using a high‑precision
   rational approximation of φ. */
static inline seq_t upper_wythoff(idx_t i)
{
    /* φ ≈ 1.618033988749894848204586834...  */
    /* Use the fraction 16180339887498948/10000000000000000 ≈ φ. */
    static const idx_t P = 16180339887498948ULL;
    static const idx_t Q = 10000000000000000ULL;

    /* Compute floor(i*φ) using 128‑bit arithmetic. */
    __int128 prod = (__int128)P * i;
    idx_t fl_phi = (idx_t)(prod / Q);

    /* t(i) = i + floor(i*φ)  */
    seq_t res = (seq_t)i + (seq_t)fl_phi;
    return res;
}

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
