/*
 * A040286  Continued fraction for sqrt(304).
 * https://oeis.org/A040286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040286: continued‑fraction terms of sqrt(304)  
   a(0) = 17, thereafter the terms repeat with period 12.  
   Computation uses the standard periodic‑continued‑fraction algorithm
   for quadratic irrationals. */

seq_t a(idx_t n)
{
    /* a(0) is the integer part of sqrt(304) */
    if (n == 0)
        return (seq_t)17;

    /* integer constants for the computation */
    const uint64_t N = 304;       /* the non‑square integer */
    const uint64_t a0 = 17;       /* floor(sqrt(304)) */

    uint64_t m = 0;              /* initial m₀ */
    uint64_t d = 1;              /* initial d₀ */
    uint64_t a_val = a0;         /* current a_k */

    /* iterate until the desired index is reached */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* m_{k+1} = d_k * a_k - m_k */
        uint64_t m_next = d * a_val - m;

        /* d_{k+1} = (N - m_{k+1}²) / d_k */
        uint64_t d_next = (N - m_next * m_next) / d;

        /* a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1}) */
        uint64_t a_next = (a0 + m_next) / d_next;

        /* advance to the next step */
        m = m_next;
        d = d_next;
        a_val = a_next;
    }

    return (seq_t)a_val;
}
