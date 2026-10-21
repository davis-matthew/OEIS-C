/*
 * A064864  Period of continued fraction for sqrt(13)*n.
 * https://oeis.org/A064864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = period length of the simple continued fraction for
 *            sqrt(13*n^2)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute integer square root of an idx_t (uint64_t) value using binary search */
static idx_t isqrt_idx(idx_t n) {
    idx_t low = 0;
    idx_t high = n;
    idx_t res = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            res = 0;
            low = 1;
            continue;
        }
        if (mid <= n / mid) {          /* mid^2 <= n  */
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n) {
    /* Compute m = 13 * n^2  (idx_t guarantees 64‑bit precision) */
    idx_t m = 13 * n * n;

    /* Floor of sqrt(m) */
    idx_t a0 = isqrt_idx(m);
    idx_t two_a0 = a0 + a0;

    /* Continued‑fraction variables (all idx_t for safety) */
    idx_t m_k = 0;          /* m₀ */
    idx_t d_k = 1;          /* d₀ */
    idx_t a_k = a0;         /* a₀ */
    idx_t period = 0;       /* period length */

    /* Loop until the periodic part repeats (a_k == 2*a0) */
    while (1) {
        idx_t m_next = d_k * a_k - m_k;
        idx_t d_next = (m - m_next * m_next) / d_k;
        idx_t a_next = (a0 + m_next) / d_next;

        period++;

        if (a_next == two_a0)
            break;

        /* Advance to next step */
        m_k = m_next;
        d_k = d_next;
        a_k = a_next;
    }

    return (seq_t)period;
}
