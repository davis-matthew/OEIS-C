/*
 * A040776  Continued fraction for sqrt(805).
 * https://oeis.org/A040776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040776: Continued fraction coefficients for sqrt(805).
   a(0) = 28 (floor(sqrt(805))).  Subsequent terms are produced by the
   standard continued‑fraction recurrence:
     a0 = floor(sqrt(D))                     (D = 805)
     m0 = 0, d0 = 1
     ai+1 = floor((a0 + mi)/di)
     mi+1 = di*ai – mi
     di+1 = (D – mi+1*mi+1) / di
   The period ends when ai+1 == 2*a0.  The sequence is periodic with that
   last term; the period length for D = 805 is 18.  The implementation
   caches the period on first call. */

seq_t a(idx_t n) {
    const seq_t D   = 805;
    const seq_t a0  = 28;       /* floor(sqrt(805)) */
    static seq_t period[64];
    static idx_t period_len = 0;

    /* Compute the period once */
    if (period_len == 0) {
        seq_t m = 0, d = 1, a_cur = a0;
        idx_t i = 0;
        while (1) {
            seq_t m_next = d * a_cur - m;
            seq_t d_next = (D - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;
            period[i++] = a_next;
            if (a_next == 2 * a0) break;   /* period terminates */
            m = m_next;
            d = d_next;
            a_cur = a_next;
        }
        period_len = i;
    }

    if (n == 0) return a0;          /* a(0) is the integer part */
    /* For n ≥ 1, index into the periodic part */
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
