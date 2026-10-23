/*
 * A040832  Continued fraction for sqrt(862).
 * https://oeis.org/A040832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040832: the partial quotients of the continued fraction of sqrt(862). */
/*   a(0) = ⌊√862⌋ = 29               */
/*   For n ≥ 1 :  a(n) is the (n‑th) term after the initial 29, repeating with the
 *                Pisot period of the continued fraction of √862. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/*  The function follows the standard recurrence for
 *  quadratic irrationals:

        m_k = d_{k-1} * a_{k-1} – m_{k-1}
        d_k = (D – m_k^2) / d_{k-1}
        a_k = ⌊(a_0 + m_k) / d_k⌋

   The period ends when a_k = 2*a_0 (here 58).  The first call
 *  precomputes the whole period into a static array; subsequent calls
 *  simply return the requested term modulo that period. */

seq_t a(idx_t n)
{
    /*  Large degree of certainty that the period of √862 is < 200,
     *  thus a static buffer is sufficient and avoids realloc hazard
     *  for arbitrary‑precision seq_t.  The array size may be increased
     *  if a longer period is unexpectedly needed. */
    static seq_t period[200];
    static size_t period_len = 0;
    static int initialized = 0;
    const seq_t D = 862;          /* the radicand */
    const seq_t a0 = 29;          /* ⌊√D⌋ */

    /*  Initialise on first use.  The recurrence stops when a_k = 2*a0. */
    if (!initialized) {
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;
        do {
            /* Apply recurrence */
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period[period_len++] = a;
        } while (a != 2 * a0);
        initialized = 1;
    }

    /*  Indexing follows OEIS convention: a(0)=29, a(1)=first periodic term. */
    if (n == 0)
        return a0;

    /*  The period is indexed from a(1).  Apply modulo arithmetic with care
     *  for large idx_t values. */
    idx_t idx = (n - 1) % (idx_t)period_len;
    return period[(size_t)idx];
}
