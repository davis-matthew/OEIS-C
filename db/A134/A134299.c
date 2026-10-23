/*
 * A134299  Maximal length of a sequence such that v(0)=n, v(k+2) =
 *          v(k)-v(k+1), v(k) >= 0.
 * https://oeis.org/A134299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   maximal length of a non‑negative sequence with v(0)=n and
 *            v(k+2)=v(k)-v(k+1). Computed by exhaustive search over v(1)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n must be at least 1 – for safety return 0 if 0 is given. */
    if (n == 0) return 0;

    /* The optimal second term m is always ≥ n/2, so we only scan
       half of the range.  This keeps the running time O(n * a(n)),
       well within the limits for n up to about 10 000. */
    idx_t m_start = (n + 1) / 2;   /* ceil(n/2)   */
    seq_t best = 2;                /* at least v0 and v1 are present */

    for (idx_t m = m_start; m <= n; ++m) {
        seq_t v_prev = (seq_t)n;
        seq_t v_curr = (seq_t)m;
        seq_t len    = 2;          /* count v0 and v1 */

        while (v_prev >= v_curr) { /* next term will be non‑negative */
            seq_t v_next = v_prev - v_curr;
            ++len;
            v_prev = v_curr;
            v_curr = v_next;
        }
        if (len > best) best = len;
    }
    return best;
}
